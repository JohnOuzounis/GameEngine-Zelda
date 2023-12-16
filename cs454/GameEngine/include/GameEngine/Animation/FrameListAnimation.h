#pragma once
#include <GameEngine/Animation/Animation.h>
#include <GameEngine/Animation/MovingAnimation.h>
#include <vector>

namespace GameEngine {
class FrameListAnimation : public MovingAnimation {
   public:
	using Frames = std::vector<unsigned>;

   protected:
	Frames frames;

   public:
	const Frames& GetFrames(void) const { return frames; }
	void SetFrames(const Frames& f) { frames = f; }

	Animation* Clone(void) const override {
		return new FrameListAnimation(id, frames, GetReps(), GetDx(), GetDy(),
									  GetDelay());
	}
	FrameListAnimation(const std::string& _id,
					   const Frames& _frames,
					   unsigned r,
					   int dx,
					   int dy,
					   unsigned d)
		: frames(_frames), MovingAnimation(id, r, dx, dy, d) {}
};
}