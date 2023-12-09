#include <GameEngine/App.h>
#include <GameEngine/Camera.h>
#include <GameEngine/Graphics/Renderer.h>
#include <GameEngine/Graphics/Texture.h>
#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Input.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/Resources.h>
#include <GameEngine/System.h>
#include <iostream>
#include "Tilelayer.h"

using namespace GameEngine;
using namespace GameEngine::Json;
using namespace GameEngine::Graphics;
using namespace app;

class Unit1 : public App {
   private:
	Window* window = nullptr;

	Tilemap* map;
	Image* displayBuffer = nullptr;
	Renderer* renderer;

	int speed = 0;
	int scaleX = 0;
	int scaleY = 0;

	void Init();
	void Input();
	void Render();

   public:
	virtual void Initialise() override {
		System::Init();
		Resources::Get().SetAssetPath(
			"../../../../../../cs454/UnitTests/Unit1/assets/");
		Resources::Get().SetRootPath(
			"../../../../../../cs454/UnitTests/Unit1/");
	}
	virtual void Clear() override {
		delete window;
		System::CleanUp();
	}
	virtual void Load() override { Init(); }
};
