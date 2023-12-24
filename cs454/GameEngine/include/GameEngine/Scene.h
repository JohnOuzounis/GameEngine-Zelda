#pragma once
#include <functional>
namespace GameEngine {

class Scene {
   public:
	using PauseGame = std::function<void(double)>;
	using ResumeGame = std::function<void(void)>;

   protected:
	PauseGame pause;
	ResumeGame resume;

   public:
	virtual ~Scene() {}

	virtual void Save() = 0;
	virtual void Load() = 0;
	virtual void CleanUp() = 0;

	void SetPause(PauseGame p) { pause = p; }
	void SetResume(ResumeGame r) { resume = r; }
	void Pause(double t) { (pause)(t); }
	void Resume() { (resume)(); }

	virtual void Render() {}
	virtual void ProgressAnimations() {}
	virtual void Input() {}
	virtual void AI() {}
	virtual void Physics() {}
	virtual void CollisionChecking() {}
	virtual void CommitDestructions() {}
	virtual void UserCode() {}
	virtual void PauseResume() {}
};

}  // namespace GameEngine