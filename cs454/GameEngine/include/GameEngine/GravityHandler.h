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
	template <typename T>
	void SetOnStartFalling(const OnSolidGroundPred& f) {
		onStartFalling = f;
	}
	template <typename T>
	void SetOnStopFalling(const T& f) {
		onStopFalling = f;
	}
	template <typename T>
	void SetOnSolidGround(const T& f) {
		onSolidGround = f;
	}
	void Reset(void) { isFalling = false; }
	void Check(const Graphics::Rect& r);

	void SetGravity(bool enable) { gravityAddicted = enable; }
	bool GetGravity() const { return gravityAddicted; }
};
}  // namespace GameEngine