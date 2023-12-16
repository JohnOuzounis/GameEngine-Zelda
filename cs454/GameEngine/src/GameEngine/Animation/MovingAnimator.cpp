#include <GameEngine/Animation/MovingAnimator.h>

using namespace GameEngine;

void MovingAnimator::Progress(double currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= anim->GetDelay()) {
		lastTime += anim->GetDelay();
		NotifyAction(*anim);
		if (!anim->IsForever() && ++currRep == anim->GetReps()) {
			state = FINISHED;
			NotifyStopped();
			return;
		}
	}
}
