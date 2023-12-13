#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Renderer.h>
#include <GameEngine/Graphics/Texture.h>
#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Input.h>
#include <GameEngine/Scene.h>
#include <GameEngine/SceneManager.h>
#include <GameEngine/System.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/StringProperty.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <GameEngine/Resources.h>
#include <GameEngine/Audio.h>
#include "TilemapMaker.h"

class Scene1 : public GameEngine::Scene {
   public:
	GameEngine::Graphics::Window* window;
	GameEngine::Graphics::Renderer* renderer;

	GameEngine::Graphics::Tilemap* map = nullptr;
	GameEngine::Graphics::Image* displayBuffer = nullptr;

	GameEngine::Audio* audio = nullptr;

	int speed = 0;
	int scaleX = 0;
	int scaleY = 0;

	Scene1(GameEngine::Graphics::Window* window,
		   GameEngine::Graphics::Renderer* renderer)
		: window(window), renderer(renderer) {}

	virtual void Load() override {
		using namespace GameEngine::Graphics;
		using namespace GameEngine::Json;
		using namespace GameEngine;

		Configurator appConfig;
		appConfig.Configure(Resources::Get().GetResource("config/app.json"));
		std::string mapName = ((StringProperty*)appConfig.GetConfigurations()
								   ->GetProperties()
								   .find("map")
								   ->second)
								  ->GetValueStripped();
		speed = (int)((NumericProperty*)appConfig.GetConfigurations()
						  ->GetProperties()
						  .find("scrollspeed")
						  ->second)
					->GetValue();
		scaleX = (int)((NumericProperty*)appConfig.GetConfigurations()
						   ->GetProperties()
						   .find("scaleX")
						   ->second)
					 ->GetValue();
		scaleY = (int)((NumericProperty*)appConfig.GetConfigurations()
						   ->GetProperties()
						   .find("scaleY")
						   ->second)
					 ->GetValue();
		std::string music = ((StringProperty*)appConfig.GetConfigurations()
								   ->GetProperties()
								   .find("backgroundmusic")
								   ->second)
								  ->GetValueStripped();
		audio = new Audio();
		audio->Load(Resources::Get().GetAsset(music));
		audio->SetLoop(true);

		TilemapMaker maker;
		Configurator mapConfig;
		mapConfig.Subscibe(&maker);
		mapConfig.Configure(Resources::Get().GetResource(mapName));
		map = maker.MakeTilemap();
		map->SetView({0, 0, window->GetWidth(), window->GetHeight()});
		Image* im = map->GetTilemap();
		im->Scale(im->GetWidth() * scaleX, im->GetHeight() * scaleY);
	}

	virtual void Save() override {}

	virtual void CleanUp() override { 
		if (map)
			GameEngine::System::Destroy(map);
		if (displayBuffer)
			GameEngine::System::Destroy(displayBuffer);
		if (audio)
			GameEngine::System::Destroy(audio);
	}

	virtual void Render() override {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		if (displayBuffer)
			System::Destroy(displayBuffer);
		displayBuffer = Image::Create(window->GetWidth(), window->GetHeight(),
									  {0, 0, 0, 255});
		map->Display(*displayBuffer, {0, 0, displayBuffer->GetWidth(),
									  displayBuffer->GetHeight()});

		Texture tex(renderer->GetRenderer(), *displayBuffer);

		renderer->Clear();
		renderer->Copy(
			tex, {0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()},
			{0, 0, window->GetWidth(), window->GetHeight()});
		//renderer->Render();
	}

	virtual void Input() override {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		if (Input::GetKeyDown(Event::P)) {
			if (audio->IsPlaying())
				audio->Stop();
			else
				audio->Play();
		}
		if (Input::GetKeyDown(Event::Plus)) {
			audio->SetVolume(audio->GetVolume() + 1);
		}
		if (Input::GetKeyDown(Event::Minus)) {
			audio->SetVolume(audio->GetVolume() - 1);
		}

		if (Input::GetKeyDown(Event::Right))
			map->Scroll(speed, 0);
		if (Input::GetKeyDown(Event::Left))
			map->Scroll(-speed, 0);
		if (Input::GetKeyDown(Event::Down))
			map->Scroll(0, speed);
		if (Input::GetKeyDown(Event::Up))
			map->Scroll(0, -speed);
		if (Input::GetKeyDown(Event::Home))
			map->SetView({0, 0, map->GetView().width, map->GetView().height});
		if (Input::GetKeyDown(Event::End)) {
			map->SetView({map->GetTilemap()->GetWidth(),
						  map->GetTilemap()->GetHeight(), map->GetView().width,
						  map->GetView().height});
			map->Scroll(1, 1);
		}

		if (Input::GetMouse(Input::MouseLeft)) {
			int x = Input::GetMouseState().mouseX;
			int y = Input::GetMouseState().mouseY;

			if (x > window->GetWidth() / 2)
				map->Scroll(speed, 0);
			if (x <= window->GetWidth() / 2)
				map->Scroll(-speed, 0);
			if (y > window->GetHeight() / 2)
				map->Scroll(0, speed);
			if (y <= window->GetHeight() / 2)
				map->Scroll(0, -speed);
		}
	}
};