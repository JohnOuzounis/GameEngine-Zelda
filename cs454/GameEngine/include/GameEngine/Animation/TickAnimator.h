#pragma once
#include <GameEngine/Animation/TickAnimation.h>
#include <GameEngine/Animation/Animator.h>

namespace GameEngine {
class TickAnimator : public Animator {
   protected:
	TickAnimation* anim = nullptr;
	unsigned currRep = 0;
	unsigned elapsedTime = 0;  // keep track of time passed between triggers
   public:
	void Progress(double currTime) override;

	unsigned GetCurrRep(void) const { return currRep; }
	unsigned GetElapsedTime(void) const { return elapsedTime; }

	float GetElapsedTimeNormalised(void) const {
		return float(elapsedTime) / float(anim->GetDelay());
	}

	void Start(const TickAnimation& a, double t) {
		anim = (TickAnimation*)a.Clone();
		lastTime = t;
		state = RUNNING;
		currRep = 0;
		elapsedTime = 0;
		NotifyStarted();
	}

	TickAnimator(void) = default;
};

}  // namespace GameEngine