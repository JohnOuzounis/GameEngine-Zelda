#pragma once
#include <GameEngine/Animation/Animation.h>
#include <GameEngine/Animation/MovingAnimation.h>

namespace GameEngine {
class FrameRangeAnimation : public MovingAnimation {
   protected:
	unsigned start = 0, end = 0;

   public:
	unsigned GetStartFrame(void) const { return start; }
	FrameRangeAnimation& SetStartFrame(unsigned v) {
		start = v;
		return *this;
	}

	unsigned GetEndFrame(void) const { return end; }
	FrameRangeAnimation& SetEndFrame(unsigned v) {
		end = v;
		return *this;
	}

	Animation* Clone(void) const override {
		return new FrameRangeAnimation(id, start, end, GetReps(), GetDx(),
									   GetDy(), GetDelay());
	}

	FrameRangeAnimation(const std::string& _id,
						unsigned s,
						unsigned e,
						unsigned r,
						int dx,
						int dy,
						int d)
		: start(s), end(e), MovingAnimation(id, r, dx, dy, d) {}
};

}