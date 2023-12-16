#pragma once
#include <GameEngine/Animation/Animator.h>
#include <GameEngine/Animation/MovingAnimation.h>

namespace GameEngine {
class MovingAnimator : public Animator {
   protected:
	MovingAnimation* anim = nullptr;
	unsigned currRep = 0;  // animation state
   public:
	virtual void Progress(double currTime) override;
	auto GetAnim(void) const -> const MovingAnimation& { return *anim; }

	void Start(MovingAnimation* a, double t) {
		anim = a;
		lastTime = t;
		state = RUNNING;
		currRep = 0;
		NotifyStarted();
	}

	MovingAnimator(void) = default;
};
}  // namespace GameEngine