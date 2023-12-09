#include <GameEngine/Game.h>

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
	Render();
	Input();
	ProgressAnimations();
	AI();
	Physics();
	CollisionChecking();
	UserCode();
	CommitDestructions();
}
