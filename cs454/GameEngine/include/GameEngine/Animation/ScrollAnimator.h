#pragma once
#include <GameEngine/Animation/Animator.h>
#include <GameEngine/Animation/ScrollAnimation.h>

namespace GameEngine {
class ScrollAnimator : public Animator {
   protected:
	ScrollAnimation* anim = nullptr;
	unsigned currIndex = 0;

   public:
	unsigned GetCurrIndex() const { return currIndex; }
	virtual void Progress(double currTime) override;

	void Start(ScrollAnimation* a, double t) {
		anim = a;
		lastTime = t;
		state = RUNNING;
		currIndex = 0;
		NotifyStarted();
	}

	ScrollAnimator() = default;
};
}  // namespace GameEngine