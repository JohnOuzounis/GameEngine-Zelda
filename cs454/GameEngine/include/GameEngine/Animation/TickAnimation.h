#pragma once
#include <GameEngine/Animation/Animation.h>
#include <cassert>

namespace GameEngine {

class TickAnimation : public Animation {
   protected:
	double delay = 0;
	unsigned reps = 1;
	bool isDiscrete = true;	 // false: when used for custom timed actions
	bool Inv(void) const { return isDiscrete || reps == 1; }

   public:
	double GetDelay(void) const { return delay; }
	TickAnimation& SetDelay(double v) {
		delay = v;
		return *this;
	}

	unsigned GetReps(void) const { return reps; }
	TickAnimation& SetReps(unsigned n) {
		reps = n;
		return *this;
	}

	bool IsForever(void) const { return !reps; }
	TickAnimation& SetForever(void) {
		reps = 0;
		return *this;
	}

	bool IsDiscrete(void) const { return isDiscrete; }

	Animation* Clone(void) const override {
		return new TickAnimation(id, delay, reps, true);
	}

	TickAnimation(const std::string& _id, double d, unsigned r, bool discrete)
		: Animation(_id), delay(d), reps(r), isDiscrete(discrete) {
		assert(Inv());
	}
};

}  // namespace GameEngine