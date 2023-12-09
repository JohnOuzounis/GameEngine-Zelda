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

	void Follow(Graphics::Rect* object) { follow = object; }
	void SetOffset(int dx, int dy) { offset.x = dx, offset.y = dy; }

	const Graphics::Rect& GetPositionClamped(int width, int height) {
		if (follow) {
			position.x = Math::Clamp(follow->x - offset.x, 0,
									 width - position.width-1);
			position.y = Math::Clamp(follow->y - offset.y, 0,
									 height - position.height-1);
		}
		else {
			position.x = Math::Clamp(position.x, 0,
				width - position.width - 1);
			position.y = Math::Clamp(position.y, 0,
				height - position.height - 1);
		}
		return position;
	}

	const Graphics::Rect& GetPosition() { return position; }
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