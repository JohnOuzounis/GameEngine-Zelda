#include <GameEngine/Animation/Animator.h>
#include <GameEngine/Animation/AnimatorManager.h>
using namespace GameEngine;

void Animator::Finish(bool isForced) {
	if (!HasFinished()) {
		state = isForced ? STOPPED : FINISHED;
		NotifyStopped();
	}
}

void Animator::Stop(void) {
	Finish(true);
}

void Animator::NotifyStopped(void) {
	AnimatorManager::GetSingleton().MarkAsSuspended(this);
	if (onFinish)
		(onFinish)(this);
}

void GameEngine::Animator::NotifyStarted(void) {
	AnimatorManager::GetSingleton().MarkAsRunning(this);
	if (onStart)
		(onStart)(this);
}

void Animator::NotifyAction(const Animation& anim) {
	if (onAction)
		(onAction)(this, anim);
}

void Animator::TimeShift(double offset) {
	lastTime += offset;
}

Animator::Animator(void) {
	AnimatorManager::GetSingleton().Register(this);
}

Animator::~Animator(void) {
	this->Stop();
	AnimatorManager::GetSingleton().Cancel(this);
}
