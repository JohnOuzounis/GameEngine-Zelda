#pragma once
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Math.h>

namespace GameEngine {

class Camera {
   private:
	Graphics::Rect position;
	Graphics::Rect* follow;
	Graphics::Point offset;

   public:
	Camera(int width, int height) {
		position = {0, 0, width, height};
		follow = nullptr;
	}
	Camera() : Camera(0, 0) {}

	Graphics::Rect* GetFollow() const { return follow; }
	void Follow(Graphics::Rect* object) { follow = object; }
	void SetOffset(int dx, int dy) { offset.x = dx, offset.y = dy; }
	const Graphics::Point& GetOffset() const { return offset; }

	const Graphics::Rect& GetPositionClamped(const Graphics::Rect& bounds) {
		if (follow) {
			position.x = Math::Clamp(follow->x - offset.x, bounds.x,
									 bounds.x + bounds.width - position.width-1);
			position.y = Math::Clamp(follow->y - offset.y, bounds.y,
									 bounds.y + bounds.height - position.height-1);
		}
		else {
			position.x = Math::Clamp(position.x, bounds.x,
				bounds.x + bounds.width - position.width - 1);
			position.y = Math::Clamp(position.y, bounds.y,
				bounds.y + bounds.height - position.height - 1);
		}
		return position;
	}

	const Graphics::Rect& GetPosition() {
		if (follow) {
			position.x = follow->x - offset.x;
			position.y = follow->y - offset.y;
		}
		return position;
	}
	void SetPosition(const Graphics::Rect& pos) { position = pos; }

	void Scroll(int dx, int dy) {	// use GetPositionClamped to correctly get the position
		if (!follow)				// or clamp the position of the camera manually
		{
			position.x += dx;
			position.y += dy;
		}
	}
};
}  // namespace GameEngine