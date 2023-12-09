#pragma once
#include <GameEngine/Graphics/Window.h>

namespace GameEngine {

class Scene {
   public:
	virtual ~Scene() {}

	virtual void Save() = 0;
	virtual void Load() = 0;
	virtual void CleanUp() = 0;

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