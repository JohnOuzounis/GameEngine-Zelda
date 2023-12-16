#include <GameEngine/Animation/FrameRangeAnimator.h>
#include <cassert>
using namespace GameEngine;

void FrameRangeAnimator::Progress(double currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		if (currFrame == anim->GetEndFrame()) {
			assert(anim->IsForever() || currRep < anim->GetReps());
			currFrame = anim->GetStartFrame();	// flip to start
		} else
			++currFrame;
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (currFrame == anim->GetEndFrame())
			if (!anim->IsForever() && ++currRep == anim->GetReps()) {
				state = FINISHED;
				NotifyStopped();
				return;
			}
	}
}
