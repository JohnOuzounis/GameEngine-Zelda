#pragma once
#include <GameEngine/Time.h>
#include <GameEngine/Game.h>
#include <functional>
namespace GameEngine {

class Scene {
   protected:
	app::Game& game;

   public:
	Scene(app::Game& game) : game(game) {}
	virtual ~Scene() {}

	virtual void Save() = 0;
	virtual void Load() = 0;
	virtual void CleanUp() = 0;

	void AddPauseResume(const app::Game::Action& p) {
		game.AddPauseResume(p, false);
	}

	void Pause() {
		game.Pause(Time::getTime());
	}

	void Resume() {
		game.Resume();
	}

	bool IsPaused() const { return game.IsPaused(); }

	virtual void Render() {}
	virtual void ProgressAnimations() {}
	virtual void Input() {}
	virtual void AI() {}
	virtual void Physics() {}
	virtual void CollisionChecking() {}
	virtual void CommitDestructions() {}
	virtual void UserCode() {}
};

}  // namespace GameEngine