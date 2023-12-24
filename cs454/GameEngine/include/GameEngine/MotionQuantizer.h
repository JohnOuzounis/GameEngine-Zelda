#pragma once
#include <functional>
#include <GameEngine/Graphics/Rect.h>

namespace GameEngine {

template <typename Tnum>
int number_sign(Tnum x) {
	return x > 0 ? 1 : x < 0 ? -1 : 0;
}

// generic quantizer, can be used to filter motion with any terrain
// motion filtering function
class MotionQuantizer {
   public:
	using Mover = std::function<void(const Graphics::Rect& r, int* dx, int* dy)>;

   protected:
	int horizMax = 0, vertMax = 0;
	Mover mover;  // filters requested motion too!
	bool used = false;

   public:
	MotionQuantizer& SetUsed(bool val) {
		used = val;
		return *this;
	}

	MotionQuantizer& SetRange(int h, int v) {
		horizMax = h, vertMax = v;
		used = true;
		return *this;
	}

	MotionQuantizer& SetMover(const Mover& f) {
		mover = f;
		return *this;
	}

	void Move(const Graphics::Rect& r, int* dx, int* dy);

	MotionQuantizer(void) = default;
	MotionQuantizer(const MotionQuantizer&) = default;
};
}  // namespace GameEngine