#include <GameEngine/LatelyDestroyable.h>
using namespace GameEngine;

DestructionManager DestructionManager::singleton;

void LatelyDestroyable::Delete(void) {
	assert(!dying);
	dying = true;
	delete this;
}

void DestructionManager::Register(LatelyDestroyable* d) {
	assert(!d->IsAlive());
	dead.push_back(d);
}

void DestructionManager::Commit(void) {
	for (auto* d : dead) {
		auto callback = d->onDestroy;
		d->Delete();
		callback();
	}
	dead.clear();
}
