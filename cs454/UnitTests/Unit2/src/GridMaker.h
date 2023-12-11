#pragma once
#include <GameEngine/Graphics/Gridmap.h>
#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <string>
#include <vector>

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
			(GameEngine::Json::AggregateProperty*)configs.find("gridmap")->second;
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
		GameEngine::Graphics::Gridmap::GridIndex* indexes = this->map->GetGrid();
		for (size_t i = 0; i < this->map->GetTotal(); i++) {
			map->Add(
				new GameEngine::Json::NumericProperty("_id", indexes[i]));
		}
		configs["gridmap"] = gridmap;
		config.WriteFile();
	}

	void ComputeTileGridBlocks1(const GameEngine::Graphics::Tilemap* map,
								GameEngine::Graphics::Gridmap* gridmap) {
		this->map = gridmap;

		GameEngine::Graphics::Gridmap::GridIndex* grid = gridmap->GetGrid();
		for (auto row = 0; row < map->GetRows(); ++row)
			for (auto col = 0; col < map->GetColumns(); ++col) {
				memset(grid,
					   IsTileIndexAssumedEmpty(map->GetTile(row, col))
						   ? GRID_EMPTY_TILE
						   : GRID_SOLID_TILE,
					   gridmap->GetGridTilesPerTile());
				grid += gridmap->GetGridTilesPerTile();
			}
	}
};