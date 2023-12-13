#include <GameEngine/Game.h>
#include <GameEngine/Time.h>
#include <GameEngine/Input.h>
#include <GameEngine/Graphics/Event.h>

using namespace GameEngine::app;
using namespace GameEngine::Graphics;

void GameEngine::app::Game::MainLoop()
{
	while (!IsFinished())
	{
		MainLoopIteration();
	}
}

void GameEngine::app::Game::MainLoopIteration()
{
	Time::Update();
	Render();

	Input::ClearEvents();
	while (Event::GetEvent().Poll()) {
		if (Event::GetEvent().GetType() == Event::Quit)
			Quit();

		Input::HandleEvent();
	}
	Input();

	ProgressAnimations();
	AI();
	Physics();
	CollisionChecking();
	UserCode();
	CommitDestructions();
}
