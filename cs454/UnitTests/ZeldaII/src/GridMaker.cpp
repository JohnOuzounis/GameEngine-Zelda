#include "../../Unit2/src/GridMaker.h"
#include "../../Unit1/src/TilemapMaker.h"

#include <GameEngine/JSON/StringProperty.h>
#include <GameEngine/Resources.h>
#include <GameEngine/System.h>

using namespace GameEngine;
using namespace GameEngine::Json;
using namespace GameEngine::Graphics;

int main(int argc, char** argv) {
	System::Init();

	Resources::Get().SetRootPath("../../../../../../cs454/UnitTests/ZeldaII/");
	Resources::Get().SetAssetPath(
		"../../../../../../cs454/UnitTests/ZeldaII/assets/");
	Configurator appConfig;
	appConfig.Configure(Resources::Get().GetResource("config/app.json"));
	std::string path = ((StringProperty*)appConfig.GetConfigurations()
							->GetProperties()
							.find("map")
							->second)
						   ->GetValueStripped();

	TilemapMaker tilemaker;
	Configurator config;
	config.Subscibe(&tilemaker);
	config.Configure(Resources::Get().GetResource(path));

	int width =
		(int)((NumericProperty*)((AggregateProperty*)config.GetConfigurations()
									 ->GetProperties()
									 .find("gridmap")
									 ->second)
				  ->GetProperties()
				  .find("tilewidth")
				  ->second)
			->GetValue();
	int height =
		(int)((NumericProperty*)((AggregateProperty*)config.GetConfigurations()
									 ->GetProperties()
									 .find("gridmap")
									 ->second)
				  ->GetProperties()
				  .find("tileheight")
				  ->second)
			->GetValue();

	GridMaker maker(Resources::Get().GetResource(path));
	Tilemap* map = tilemaker.MakeTilemap();
	GameEngine::Graphics::Gridmap* grid = new GameEngine::Graphics::Gridmap(
		map->GetRows(), map->GetColumns(), map->GetTileWidth(),
		map->GetTileHeight(), width, height);

	maker.ComputeTileGridBlocks1(map, grid);
	maker.Save();

	return EXIT_SUCCESS;
}