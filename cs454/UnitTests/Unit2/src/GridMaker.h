#pragma once
#include <GameEngine/Graphics/Gridmap.h>
#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <functional>
#include <string>
#include <vector>
#include "TileColorHolder.h"

class GridMaker {
	GameEngine::Json::ArrayProperty* emptyTiles = nullptr;
	GameEngine::Json::Configurator config;
	GameEngine::Graphics::Gridmap* map = nullptr;

	bool IsTileIndexAssumedEmpty(int index) {
		for (auto& tile : emptyTiles->GetProperties()) {
			if ((int)((GameEngine::Json::NumericProperty*)tile)->GetValue() ==
				index)
				return true;
		}
		return false;
	}

	bool ComputeIsGridIndexEmpty(GameEngine::Graphics::Image* gridElement,
								 GameEngine::Graphics::Color transColor,
								 unsigned char solidThreshold) {
		auto n = 0;
		gridElement->AccessPixels([transColor, &n, gridElement](Uint32 pixel) {
			auto c = gridElement->GetPixelColor(pixel);
			if (*c != transColor && !IsTileColorEmpty(*c))
				++n;
			delete c;
		});
		return n <= solidThreshold;
	}

   public:
	GridMaker(std::string path) {
		config.Configure(path);
		GameEngine::Json::AggregateProperty* tilemap =
			(GameEngine::Json::AggregateProperty*)config.GetConfigurations()
				->GetProperties()
				.find("tilemap")
				->second;
		emptyTiles = (GameEngine::Json::ArrayProperty*)tilemap->GetProperties()
						 .find("emptytiles")
						 ->second;
	}

	void Save() {
		GameEngine::Json::AggregateProperty::Properties& configs =
			config.GetConfigurations()->GetProperties();

		GameEngine::Json::AggregateProperty* gridmap =
			(GameEngine::Json::AggregateProperty*)configs.find("gridmap")
				->second;
		((GameEngine::Json::NumericProperty*)gridmap->GetProperties()
			 .find("tilewidth")
			 ->second)
			->SetValue(this->map->GetGridTileWidth());
		((GameEngine::Json::NumericProperty*)gridmap->GetProperties()
			 .find("tileheight")
			 ->second)
			->SetValue(this->map->GetGridTileHeight());
		auto& it = gridmap->GetProperties().find("map");

		GameEngine::Json::ArrayProperty* map =
			(GameEngine::Json::ArrayProperty*)it->second;
		map->Delete();
		GameEngine::Graphics::Gridmap::GridIndex* indexes =
			this->map->GetGrid();
		for (size_t i = 0; i < this->map->GetTotal(); i++) {
			map->Add(new GameEngine::Json::NumericProperty("_id", indexes[i]));
		}
		configs["gridmap"] = gridmap;
		config.WriteFile();
	}

	void ComputeTileGridBlocks2(const GameEngine::Graphics::Tilemap* map,
								GameEngine::Graphics::Gridmap* gridmap,
								GameEngine::Graphics::Image* tileset,
								GameEngine::Graphics::Color transcolor,
								unsigned char solidThreshold) {
		this->map = gridmap;

		auto tileElem = GameEngine::Graphics::Image::Create(
			map->GetTileWidth(), map->GetTileHeight(), transcolor);
		auto gridElem = GameEngine::Graphics::Image::Create(
			gridmap->GetGridTileWidth(), gridmap->GetGridTileHeight(),
			transcolor);

		auto grid = gridmap->GetGrid();

		for (auto row = 0; row < map->GetRows(); ++row)
			for (auto col = 0; col < map->GetColumns(); ++col) {
				auto index = map->GetTile(row, col);
				tileset->Blit(
					{map->GetX(index), map->GetY(index), map->GetTileWidth(),
					 map->GetTileHeight()},
					*tileElem,
					{0, 0, map->GetTileWidth(), map->GetTileHeight()});
				if (IsTileIndexAssumedEmpty(index)) {
					emptyTileColors.Insert(
						*tileElem, index);	// assume tile colors to be empty
					memset(grid, GRID_EMPTY_TILE,
						   gridmap->GetGridTilesPerTile());
					grid += gridmap->GetGridTilesPerTile();
				} else {
					for (auto i = 0; i < gridmap->GetGridTilesPerTile(); ++i) {
						auto x = i % gridmap->GetGridBlockRows();
						auto y = i / gridmap->GetGridBlockRows();
						tileElem->Blit({x * gridmap->GetGridTileWidth(),
										y * gridmap->GetGridTileHeight(),
										gridmap->GetGridTileWidth(),
										gridmap->GetGridTileHeight()},
									   *gridElem,
									   {0, 0, gridmap->GetGridTileWidth(),
										gridmap->GetGridTileHeight()});
						auto isEmpty = ComputeIsGridIndexEmpty(
							gridElem, transcolor, solidThreshold);
						*grid++ = isEmpty ? GRID_EMPTY_TILE : GRID_SOLID_TILE;
					}
				}
			}
		delete tileElem;
		delete gridElem;
	}

	void ComputeTileGridBlocks1(const GameEngine::Graphics::Tilemap* map,
								GameEngine::Graphics::Gridmap* gridmap) {
		this->map = gridmap;

		GameEngine::Graphics::Gridmap::GridIndex* grid = gridmap->GetGrid();
		for (auto row = 0; row < map->GetRows(); ++row)
			for (auto col = 0; col < map->GetColumns(); ++col) {
				int gridRow =
					row * (map->GetTileHeight() / gridmap->GetGridTileHeight());
				int gridCol =
					col * (map->GetTileWidth() / gridmap->GetGridTileWidth());

				for (int i = 0; i < gridmap->GetGridBlockRows(); ++i)
					for (int j = 0; j < gridmap->GetGridBlockColumns(); ++j) {
						grid[(gridRow + i) * gridmap->GetGridColumns() +
							 (gridCol + j)] =
							IsTileIndexAssumedEmpty(map->GetTile(row, col))
								? GRID_EMPTY_TILE
								: GRID_SOLID_TILE;
					}
			}
	}
};