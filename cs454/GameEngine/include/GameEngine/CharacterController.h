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

	const Graphics::Gridmap& GetGridmap() const { return gridmap; }

	void Move(int dx, int dy);
	void AdjustToWalls(int x,
					   int y,
					   int offsetX,
					   int offsetY,
					   int width,
					   int height);
};
}  // namespace GameEngine