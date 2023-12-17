#pragma once
#include <GameEngine/Animation/Animator.h>
#include <GameEngine/Animation/MovingPathAnimation.h>

namespace GameEngine {
class MovingPathAnimator : public Animator {
   protected:
	MovingPathAnimation* anim = nullptr;
	unsigned currIndex = 0;

   public:
	unsigned GetCurrIndex() const { return currIndex; }
	virtual void Progress(double currTime) override;

	void Start(MovingPathAnimation* a, double t) {
		anim = a;
		lastTime = t;
		state = RUNNING;
		currIndex = 0;
		NotifyStarted();
	}

	MovingPathAnimator() = default;
};
}  // namespace GameEngine