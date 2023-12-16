#include <GameEngine/Animation/TickAnimator.h>
using namespace GameEngine;

void TickAnimator::Progress(double currTime) {
	if (!anim->IsDiscrete()) {	// no discrete fires in every loop!
		elapsedTime = currTime - lastTime;
		lastTime = currTime;
		NotifyAction(*anim);
	} else
		while (currTime > lastTime &&
			   (currTime - lastTime) >= anim->GetDelay()) {
			lastTime += anim->GetDelay();
			NotifyAction(*anim);
			if (!anim->IsForever() && ++currRep == anim->GetReps()) {
				state = FINISHED;
				NotifyStopped();
				return;
			}
		}
}