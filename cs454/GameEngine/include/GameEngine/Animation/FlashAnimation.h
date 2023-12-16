#pragma once
#include <GameEngine/Animation/Animation.h>

namespace GameEngine {
class FlashAnimation : public Animation {
   private:
	unsigned repetitions = 0;
	unsigned hideDelay = 0;
	unsigned showDelay = 0;

   public:
	unsigned GetRepetitions(void) const { return repetitions; }
	FlashAnimation& SetRepetitions(unsigned n) {
		repetitions = n;
		return *this;
	}

	unsigned GetHideDelay(void) const { return hideDelay; }
	FlashAnimation& SetHideDelay(unsigned d) {
		hideDelay = d;
		return *this;
	}

	unsigned GetShowDelay(void) const { return showDelay; }
	FlashAnimation& SetShowDelay(unsigned d) {
		showDelay = d;
		return *this;
	}

	Animation* Clone(void) const override {
		return new FlashAnimation(id, repetitions, hideDelay, showDelay);
	}

	FlashAnimation(const std::string& _id,
				   unsigned n,
				   unsigned show,
				   unsigned hide)
		: Animation(id), repetitions(n), hideDelay(hide), showDelay(show) {}
};
}