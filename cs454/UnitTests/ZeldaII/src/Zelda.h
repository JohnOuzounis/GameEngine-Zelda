#include <GameEngine/App.h>
#include<GameEngine/Graphics/Window.h>
#include<GameEngine/Graphics/Renderer.h>

class Zelda : public GameEngine::app::App {
   private:
	void Render();
	void Input();
	void Anim();
	void Destroy();
	void Collision();
	void AI();
	void User();

   public:
	GameEngine::Graphics::Window* window = nullptr;
	GameEngine::Graphics::Renderer* renderer = nullptr;

	virtual void Initialise() override;
	virtual void Load() override;
	virtual void Clear() override;
};