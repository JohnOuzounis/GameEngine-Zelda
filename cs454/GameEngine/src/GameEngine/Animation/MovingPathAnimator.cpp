#include <GameEngine/Animation/MovingPathAnimator.h>
using namespace GameEngine;

void GameEngine::MovingPathAnimator::Progress(double currTime) {
	while (currTime > lastTime &&
		   (currTime - lastTime) >= anim->GetPath()[currIndex].delay) {
		lastTime += anim->GetPath()[currIndex].delay;
		NotifyAction(*anim);

		if (++currIndex == anim->GetPath().size()) {
			state = FINISHED;
			NotifyStopped();
			return;
		}
	}
}