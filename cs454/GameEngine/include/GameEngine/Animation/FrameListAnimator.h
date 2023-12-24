#pragma once
#include <GameEngine/Animation/Animator.h>
#include <GameEngine/Animation/FrameListAnimation.h>

namespace GameEngine {
class FrameListAnimator : public Animator {
   protected:
	FrameListAnimation* anim = nullptr;
	unsigned currIndex = 0;
	unsigned currFrame = 0;
	unsigned currRep = 0;

   public:
	virtual void Progress(double currTime) override;
	unsigned GetCurrFrame(void) const { return currFrame; }
	unsigned GetCurrIndex(void) const { return currIndex; }
	unsigned GetCurrRep(void) const { return currRep; }
	const FrameListAnimation* GetAnimation() const { return anim; }

	void Start(FrameListAnimation* a, double t) {
		anim = a;
		lastTime = t;
		state = RUNNING;
		currIndex = 0;
		currFrame = anim->GetFrames()[currIndex];
		currRep = 0;
		NotifyStarted();
		NotifyAction(*anim);
	}

	FrameListAnimator(void) = default;
};
}  // namespace GameEngine