#include "Unit2.h"
#include <GameEngine/Input.h>
#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <GameEngine/JSON/StringProperty.h>
#include <GameEngine/SceneManager.h>
#include <GameEngine/Time.h>
#include "../../Unit1/src/Scene1.h"

using namespace GameEngine::Json;

void Unit2::Init() {
	Configurator appConfig;
	appConfig.Configure(Resources::Get().GetResource("config/app.json"));

	std::string path = ((StringProperty*)appConfig.GetConfigurations()
							->GetProperties()
							.find("map")
							->second)
						   ->GetValueStripped();
	Configurator config;
	config.Configure(Resources::Get().GetResource(path));
	AggregateProperty* gridConfig =
		(AggregateProperty*)config.GetConfigurations()
			->GetProperties()
			.find("gridmap")
			->second;

	AggregateProperty* tileConfig =
		(AggregateProperty*)config.GetConfigurations()
			->GetProperties()
			.find("tilemap")
			->second;

	int tileWidth = (int)((NumericProperty*)tileConfig->GetProperties()
							  .find("tilewidth")
							  ->second)
						->GetValue();
	int tileHeight = (int)((NumericProperty*)tileConfig->GetProperties()
							   .find("tileheight")
							   ->second)
						 ->GetValue();
	int tilemapWidth = (int)((NumericProperty*)tileConfig->GetProperties()
								 .find("width")
								 ->second)
						   ->GetValue();
	int tilemapHeight = (int)((NumericProperty*)tileConfig->GetProperties()
								  .find("height")
								  ->second)
							->GetValue();
	int gridWidth = (int)((NumericProperty*)gridConfig->GetProperties()
							  .find("tilewidth")
							  ->second)
						->GetValue();
	int gridHeight = (int)((NumericProperty*)gridConfig->GetProperties()
							   .find("tileheight")
							   ->second)
						 ->GetValue();

	ArrayProperty* array =
		(ArrayProperty*)gridConfig->GetProperties().find("map")->second;
	this->gridmap =
		new Gridmap(tilemapHeight / tileHeight, tilemapWidth / tileWidth,
					tileWidth, tileHeight, gridWidth, gridHeight);

	Gridmap::GridIndex* map = this->gridmap->GetGrid();
	for (size_t i = 0; i < gridmap->GetTotal(); i++) {
		map[i] =
			(Gridmap::GridIndex)((NumericProperty*)array->Get(i))->GetValue();
	}

	playerImage = Image::Create(tileWidth, tileHeight, {255, 255, 255, 255});
	playerPosition = {0, 0, tileWidth, tileHeight};
	actionLayer = Image::Create(tilemapWidth * tileWidth,
								tilemapHeight * tileHeight, {0, 0, 0, 255});
	actionLayer->SetColorKey({0, 0, 0, 255}, true);
	displayBuffer =
		Image::Create(this->unit1.window->GetWidth(),
					  this->unit1.window->GetHeight(), {0, 0, 0, 255});
	displayBuffer->SetColorKey({0, 0, 0, 255}, true);
}

void Unit2::Render() {
	actionLayer->Clear({0, 0, 0, 255});
	playerImage->Blit({0, 0, playerPosition.width, playerPosition.height},
					  *actionLayer, playerPosition);

	displayBuffer->Clear({0, 0, 0, 255});
	actionLayer->BlitScaled(
		((Scene1*)SceneManager::GetSceneManager().GetCurrentScene())
			->map->GetView(),
		*displayBuffer,
		{0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()});

	Texture tex(this->unit1.renderer->GetRenderer(), *displayBuffer);

	this->unit1.renderer->Copy(
		tex, {0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()},
		{0, 0, this->unit1.window->GetWidth(), this->unit1.window->GetHeight()});
	this->unit1.renderer->Render();
}

void Unit2::Input() {
	if (Input::GetMouse(Input::MouseRight)) {
		playerPosition.x = Input::GetMouseState().mouseX;
		playerPosition.y = Input::GetMouseState().mouseY;
	}
}
