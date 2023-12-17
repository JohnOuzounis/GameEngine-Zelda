#pragma once
#include <GameEngine/Animation/Animation.h>

namespace GameEngine {
class FlashAnimation : public Animation {
   private:
	unsigned repetitions = 0;
	double hideDelay = 0;
	double showDelay = 0;

   public:
	unsigned GetRepetitions(void) const { return repetitions; }
	FlashAnimation& SetRepetitions(unsigned n) {
		repetitions = n;
		return *this;
	}

	double GetHideDelay(void) const { return hideDelay; }
	FlashAnimation& SetHideDelay(double d) {
		hideDelay = d;
		return *this;
	}

	double GetShowDelay(void) const { return showDelay; }
	FlashAnimation& SetShowDelay(double d) {
		showDelay = d;
		return *this;
	}

	Animation* Clone(void) const override {
		return new FlashAnimation(id, repetitions, hideDelay, showDelay);
	}

	FlashAnimation(const std::string& _id,
				   unsigned n,
				   double show,
				   double hide)
		: Animation(_id), repetitions(n), hideDelay(hide), showDelay(show) {}
};
}