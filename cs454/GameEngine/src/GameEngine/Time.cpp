#include <GameEngine/Time.h>

using namespace GameEngine;

double Time::time = 0.0;
double Time::deltaTime = 0.05;
float Time::timeScale = 1;

double Time::lastTime = 0;
long long Time::startTime =
	std::chrono::high_resolution_clock::now().time_since_epoch().count();