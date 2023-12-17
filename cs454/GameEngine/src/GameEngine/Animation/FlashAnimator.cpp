#include <GameEngine/Animation/FlashAnimator.h>
#include <cassert>
using namespace GameEngine;

void GameEngine::FlashAnimator::Progress(double currTime) {
	while (currTime > lastTime && (currTime - lastTime) >= currDelay) {
		lastTime += currDelay;
		NotifyAction(*anim);

		if (isHidden) {
			currDelay = anim->GetHideDelay();
			isHidden = false;
		} else {
			currDelay = anim->GetShowDelay();
			isHidden = true;

			if (++currRep == anim->GetRepetitions()) {
				state = FINISHED;
				NotifyStopped();
				return;
			}
		}
	}
}