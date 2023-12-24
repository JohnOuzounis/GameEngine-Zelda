#pragma once
#include <GameEngine/Graphics/Rect.h>
#include <functional>

namespace GameEngine {
class GravityHandler {
   public:
	using OnSolidGroundPred = std::function<bool(const Graphics::Rect&)>;
	using OnStartFalling = std::function<void(void)>;
	using OnStopFalling = std::function<void(void)>;

   protected:
	bool gravityAddicted = false;
	bool isFalling = false;
	OnSolidGroundPred onSolidGround;
	OnStartFalling onStartFalling;
	OnStopFalling onStopFalling;

   public:
	
	void SetOnStartFalling(const OnStartFalling& f) {
		onStartFalling = f;
	}
	
	void SetOnStopFalling(const OnStopFalling& f) {
		onStopFalling = f;
	}
	
	void SetOnSolidGround(const OnSolidGroundPred& f) {
		onSolidGround = f;
	}
	void Reset(void) { isFalling = false; }
	void Check(const Graphics::Rect& r);

	void SetGravity(bool enable) { gravityAddicted = enable; }
	bool GetGravity() const { return gravityAddicted; }
};
}  // namespace GameEngine