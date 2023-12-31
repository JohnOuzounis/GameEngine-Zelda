#pragma once
#include <list>
#include <tuple>
#include <cassert>
#include <functional>
#include <GameEngine/Graphics/Sprite.h>

namespace GameEngine {
class CollisionChecker final {
	static CollisionChecker singleton;
   public:
	using Action = std::function<void(Graphics::Sprite* s1, Graphics::Sprite* s2)>;

   protected:
	using Entry = std::tuple<Graphics::Sprite*, Graphics::Sprite*, Action>;
	std::list<Entry> entries;
	auto Find(Graphics::Sprite* s1, Graphics::Sprite* s2)
		-> std::list<Entry>::iterator;

   public:
	void Register(Graphics::Sprite* s1, Graphics::Sprite* s2, const Action& f) {
		assert(Find(s1, s2) == entries.end());
		entries.push_back(std::make_tuple(s1, s2, f));
	}
	void Cancel(Graphics::Sprite* s1, Graphics::Sprite* s2);
	void CleanUp() { entries.clear(); }
	void RemoveDead();

	void Check(void) const;

	static auto GetSingleton(void) -> CollisionChecker& { return singleton; }
	static auto GetSingletonConst(void) -> const CollisionChecker& {
		return singleton;
	}
};
}  // namespace GameEngine