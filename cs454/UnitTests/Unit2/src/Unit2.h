#include <GameEngine/App.h>
#include <GameEngine/Resources.h>
#include <GameEngine/Graphics/Gridmap.h>
#include <GameEngine/Graphics/Image.h>
#include "../../Unit1/src/Unit1.h"

using namespace GameEngine;
using namespace GameEngine::app;

class Unit2 : public App {
	void Init();
	void Render();
	void Input();

   public:
	Unit1 unit1;
	Gridmap* gridmap;
	Image* playerImage;
	Rect playerPosition;
	Image* actionLayer;
	Image* displayBuffer;

	virtual void Initialise() override {
		unit1.Initialise();
		Resources::Get().SetAssetPath(
			"../../../../../../cs454/UnitTests/Unit2/assets/");
		Resources::Get().SetRootPath(
			"../../../../../../cs454/UnitTests/Unit2/");
	}
	virtual void Clear() override {
		unit1.Clear();
		System::Destroy(gridmap);
		System::Destroy(playerImage);
	}
	virtual void Load() override {
		unit1.Load();
		this->game = unit1.GetGame();
		this->game.AddRender(std::bind(&Unit2::Render, this), false);
		this->game.AddInput(std::bind(&Unit2::Input, this), false);
		Init();
	}
};