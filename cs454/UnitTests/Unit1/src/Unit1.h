#include <GameEngine/App.h>
#include <GameEngine/Graphics/Renderer.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Resources.h>
#include <GameEngine/System.h>

using namespace GameEngine;
using namespace GameEngine::app;
using namespace GameEngine::Graphics;

class Unit1 : public App {
   private:
	void Init();
	void Input();
	void Render();

   public:
	Window* window = nullptr;
	Renderer* renderer = nullptr;

	virtual void Initialise() override {
		System::Init();
		Resources::Get().SetAssetPath(
			"../../../../../../cs454/UnitTests/Unit1/assets/");
		Resources::Get().SetRootPath(
			"../../../../../../cs454/UnitTests/Unit1/");
	}
	virtual void Clear() override {
		System::Destroy(window);
		System::Destroy(renderer);
		System::CleanUp();
	}
	virtual void Load() override { Init(); }
};
