#include <GameEngine/Animation/ScrollAnimator.h>
using namespace GameEngine;

void GameEngine::ScrollAnimator::Progress(double currTime) {
	while (currTime > lastTime &&
		   (currTime - lastTime) >= anim->GetScroll()[currIndex].delay) {
		lastTime += anim->GetScroll()[currIndex].delay;
		NotifyAction(*anim);

		if (++currIndex == anim->GetScroll().size())
		{
			state = FINISHED;
			NotifyStopped();
			return;
		}
	}
}
