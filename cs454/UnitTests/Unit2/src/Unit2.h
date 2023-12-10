#include <GameEngine/App.h>
#include <GameEngine/Resources.h>
#include "../../Unit1/src/Unit1.h"

using namespace GameEngine;
using namespace GameEngine::app;

class Unit2 : public App {
   public:
	Unit1 unit1;

	virtual void Initialise() override {
		unit1.Initialise();
		Resources::Get().SetAssetPath(
			"../../../../../../cs454/UnitTests/Unit2/assets/");
		Resources::Get().SetRootPath(
			"../../../../../../cs454/UnitTests/Unit2/");
	}
	virtual void Clear() override { unit1.Clear(); }
	virtual void Load() override {
		unit1.Load();
		this->game = unit1.GetGame();
	}
};