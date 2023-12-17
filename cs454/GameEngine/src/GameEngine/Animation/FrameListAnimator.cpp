#include <GameEngine/Animation/FrameListAnimator.h>
using namespace GameEngine;

void GameEngine::FrameListAnimator::Progress(double currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		if (currIndex == anim->GetFrames().size()-1) {
			assert(anim->IsForever() || currRep < anim->GetReps());
			currIndex = 0;	// flip to start
		} else
			++currIndex;
		currFrame = anim->GetFrames()[currIndex];
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (currIndex == anim->GetFrames().size()-1)
			if (!anim->IsForever() && ++currRep == anim->GetReps()) {
				state = FINISHED;
				NotifyStopped();
				return;
			}
	}
}
