#pragma once
#include <chrono>

namespace GameEngine {

class Time final {
   private:
	Time() = delete;
	~Time() = delete;

   private:
	static double time;
	static double deltaTime;
	static float timeScale;

	static long long startTime;
	static double lastTime;

   public:
	/// <summary>
	/// Update should be called once at the start of every frame
	/// </summary>
	static void Update() {
		long long currentTime = std::chrono::high_resolution_clock::now()
						 .time_since_epoch()
						 .count();

		double current = static_cast<double>(currentTime - startTime) / 1000000000;
		Time::deltaTime = current - lastTime;
		Time::time += Time::deltaTime * Time::timeScale;
		lastTime = current;
	}

	/// <summary>
	/// time since start of app in seconds
	/// </summary>
	/// <returns></returns>
	static double getTime() { return Time::time; }

	static double getDeltaTime() { return Time::deltaTime*Time::timeScale; }

	static float getTimeScale() { return Time::timeScale; }
	static void setTimeScale(float timeScale) { Time::timeScale = timeScale; }
};

}  // namespace GameEngine