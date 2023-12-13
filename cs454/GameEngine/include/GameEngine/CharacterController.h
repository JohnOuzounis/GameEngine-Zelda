#pragma once
#include <GameEngine/Graphics/Gridmap.h>
#include <GameEngine/Graphics/Rect.h>

namespace GameEngine {
class CharacterController {
   private:
	Graphics::Rect& position;
	const Graphics::Gridmap& gridmap;

   public:
	CharacterController(Graphics::Rect& position,
						const Graphics::Gridmap& gridmap)
		: position(position), gridmap(gridmap) {}

	void Move(int dx, int dy);
};
}  // namespace GameEngine