#pragma once
#include <GameEngine/Time.h>
#include <functional>
namespace GameEngine {

class Scene {
   public:
	using PauseGame = std::function<void()>;
	using ResumeGame = std::function<void(void)>;

   protected:
	PauseGame pause;
	ResumeGame resume;
	bool isPaused = false;

   public:
	virtual ~Scene() {}

	virtual void Save() = 0;
	virtual void Load() = 0;
	virtual void CleanUp() = 0;

	void SetPause(PauseGame p) { pause = p; }
	void SetResume(ResumeGame r) { resume = r; }
	void Pause() {
		isPaused = true;
		Time::setTimeScale(0);
		if (pause)
			(pause)();
	}
	void Resume() {
		isPaused = false;
		Time::setTimeScale(1);
		if (resume)
			(resume)();
	}
	bool IsPaused() const { return isPaused; }

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