#pragma once
#include <GameEngine/Animation/Animator.h>
#include <GameEngine/Animation/FrameRangeAnimation.h>

namespace GameEngine {
class FrameRangeAnimator : public Animator {
   protected:
	FrameRangeAnimation* anim = nullptr;
	unsigned currFrame = 0;	 // animation state
	unsigned currRep = 0;	 // animation state
   public:
	virtual void Progress(double currTime) override;
	unsigned GetCurrFrame(void) const { return currFrame; }
	unsigned GetCurrRep(void) const { return currRep; }

	void Start(FrameRangeAnimation* a, double t) {
		anim = a;
		lastTime = t;
		state = RUNNING;
		currFrame = anim->GetStartFrame();
		currRep = 0;
		NotifyStarted();
		NotifyAction(*anim);
	}

	FrameRangeAnimator(void) = default;
};
}  // namespace GameEngine
