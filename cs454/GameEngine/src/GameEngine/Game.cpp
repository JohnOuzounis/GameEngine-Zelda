#include <GameEngine/Game.h>
#include <GameEngine/Graphics/Event.h>
#include <GameEngine/Input.h>
#include <GameEngine/Time.h>
#include <GameEngine/Debug.h>
#include <stdexcept>

using namespace GameEngine::app;
using namespace GameEngine::Graphics;

void GameEngine::app::Game::MainLoop() {
	while (!IsFinished()) {
		MainLoopIteration();
	}
}

void GameEngine::app::Game::MainLoopIteration() {
	try {
		Time::Update();
		Render();

		Input::ClearEvents();
		while (Event::GetEvent().Poll()) {
			if (Event::GetEvent().GetType() == Event::Quit)
				Quit();

			Input::HandleEvent();
		}
		Input();

		if (!IsPaused()) {
			ProgressAnimations();
			AI();
			Physics();
			CollisionChecking();
			UserCode();
			CommitDestructions();
		}
	} catch (std::exception& e) {
		return;
	}
}
