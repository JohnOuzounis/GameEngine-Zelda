#pragma once
#include <GameEngine/Animation/Animator.h>
#include <GameEngine/Animation/FlashAnimation.h>

namespace GameEngine {
class FlashAnimator : public Animator {
   protected:
	FlashAnimation* anim = nullptr;
	double currDelay = 0;
	unsigned currRep = 0;
	bool isHidden = false;

   public:
	bool IsHidden() const { return isHidden; }
	unsigned GetCurrRep() const { return currRep; }
	double GetCurrDelay() const { return currDelay; }

	virtual void Progress(double currTime) override;
	void Start(FlashAnimation* a, double t) {
		anim = a;
		lastTime = t;
		state = RUNNING;
		currDelay = anim->GetHideDelay();
		currRep = 0;
		isHidden = true;
		NotifyStarted();
	}

	FlashAnimator() = default;
};
}  // namespace GameEngine