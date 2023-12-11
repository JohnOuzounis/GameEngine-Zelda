#include <GameEngine/Game.h>
#include <GameEngine/Time.h>

using namespace GameEngine::app;

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
	Input();
	ProgressAnimations();
	AI();
	Physics();
	CollisionChecking();
	UserCode();
	CommitDestructions();
}
