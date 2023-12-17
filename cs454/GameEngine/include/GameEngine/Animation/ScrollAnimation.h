#pragma once
#include <GameEngine/Animation/Animation.h>
#include <vector>

namespace GameEngine {
struct ScrollEntry {
	int dx = 0;
	int dy = 0;
	double delay = 0;
};
class ScrollAnimation : public Animation {
   public:
	using Scroll = std::vector<ScrollEntry>;

   private:
	Scroll scroll;

   public:
	const Scroll& GetScroll(void) const { return scroll; }
	void SetScroll(const Scroll& p) { scroll = p; }

	Animation* Clone(void) const override {
		return new ScrollAnimation(id, scroll);
	}

	ScrollAnimation(const std::string& _id, const Scroll& _scroll)
		: Animation(_id), scroll(_scroll) {}
};

}  // namespace GameEngine