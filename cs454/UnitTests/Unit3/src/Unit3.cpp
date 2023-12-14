#include "Unit3.h"
#include <GameEngine/Input.h>
#include <GameEngine/System.h>
#include <GameEngine/Resources.h>
#include <GameEngine/Graphics/Texture.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/StringProperty.h>
#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/NumericProperty.h>
#include "../../Unit1/src/TilemapMaker.h"

using namespace GameEngine;
using namespace GameEngine::Json;
using namespace GameEngine::Graphics;

void Unit3::Initialise() {
	System::Init();

	Resources::Get().SetAssetPath(
		"../../../../../../cs454/UnitTests/Unit3/assets/");
	Resources::Get().SetRootPath("../../../../../../cs454/UnitTests/Unit3/");
}

void Unit3::Load() {
	window = new Window("Unit3", 100, 100, 680, 420, Window::Flags::Shown);
	renderer = new Renderer(*window, -1, Renderer::Accelerated);
	displayBuffer =
		Image::Create(window->GetWidth(), window->GetHeight(), {0, 0, 0, 255});


	this->game.SetFinished([&]() { return !window->IsOpen(); });
	this->game.AddQuit([&]() { window->Close(); }, false);
	this->game.AddRender(
		[&]() {
			displayBuffer->Clear({0, 0, 0, 255});

			bg->Display(displayBuffer, {0, 0, displayBuffer->GetWidth(),
										displayBuffer->GetHeight()});

			actionTilemap->Display(
				*displayBuffer,
				{0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()});


			Texture tex(renderer->GetRenderer(), *displayBuffer);
			renderer->Clear();
			renderer->Copy(
				tex,
				{0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()},
				{0, 0, window->GetWidth(), window->GetHeight()});
			renderer->Render();
		},
		false);

	this->game.AddInput(
		[&]() {
			int dx = 0, dy = 0;
			if (Input::GetKeyDown(Event::Left))
				dx = -1;
			if (Input::GetKeyDown(Event::Right))
				dx = 1;
			if (Input::GetKeyDown(Event::Up))
				dy = -1;
			if (Input::GetKeyDown(Event::Down))
				dy = 1;

			if (actionTilemap->CanScrollHorizontal(2*dx)) {
				bg->Scroll(dx);
				actionTilemap->Scroll(2*dx, 2*dy);
			}
		},
		false);

	Configurator appConfig;
	appConfig.Configure(Resources::Get().GetResource("config/app.json"));

	std::string mapName = ((StringProperty*)appConfig.GetConfigurations()
							   ->GetProperties()
							   .find("map")
							   ->second)
							  ->GetValueStripped();

	Configurator mapConfig;
	mapConfig.Configure(Resources::Get().GetResource(mapName));

	AggregateProperty* tilemaps =
		(AggregateProperty*)mapConfig.GetConfigurations()
			->GetProperties()
			.find("tilemap")
			->second;

	AggregateProperty* actionMap =
		(AggregateProperty*)tilemaps->GetProperties().find("action")->second;

	TilemapMaker actionMaker;
	actionMaker.Configure(*actionMap);
	actionTilemap = actionMaker.MakeTilemap();
	actionTilemap->SetView({0, 0, window->GetWidth()/2, window->GetHeight()/2});

	AggregateProperty* bgMap =
		(AggregateProperty*)tilemaps->GetProperties().find("background")->second;

	TilemapMaker bgMaker;
	bgMaker.Configure(*bgMap);
	bgTilemap = bgMaker.MakeTilemap();

	bg = new CircularBackground(bgTilemap->GetTilemap()->Copy());
	bg->SetView({0, 0, window->GetWidth()/2, window->GetHeight()/2});
	System::Destroy(bgTilemap);
}

void Unit3::Clear() {
	System::Destroy(window);
	System::Destroy(renderer);

	System::Destroy(actionTilemap);
	System::Destroy(displayBuffer);
	System::Destroy(bg);
}
