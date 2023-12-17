#pragma once
#include <list>
#include <cassert>
#include <functional>

namespace GameEngine {

class LatelyDestroyable;
class DestructionManager {
	std::list<LatelyDestroyable*> dead;
	static DestructionManager singleton;

   public:
	void Register(LatelyDestroyable* d);
	void Commit(void);
	static auto Get(void) -> DestructionManager& { return singleton; }
};

class LatelyDestroyable {
   public:
	   using OnDestroy = std::function<void()>;
   protected:
	friend class DestructionManager;
	bool alive = true;
	bool dying = false;
	OnDestroy onDestroy;

	virtual ~LatelyDestroyable() { assert(dying); }
	void Delete(void);

   public:
	void SetOnDestroy(OnDestroy onDestroy) { this->onDestroy = onDestroy; }
	bool IsAlive(void) const { return alive; }
	void Destroy(void) {
		if (alive) {
			alive = false;
			DestructionManager::Get().Register(this);
		}
	}
	LatelyDestroyable(void) = default;
};

}  // namespace GameEngine