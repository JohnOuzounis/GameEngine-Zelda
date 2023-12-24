#include <GameEngine/CharacterController.h>
#include <GameEngine/Math.h>
using namespace GameEngine;
using namespace GameEngine::Graphics;

void GameEngine::CharacterController::Move(int dx, int dy) {
	do {
		auto sign_x = Math::Sign(dx);
		auto sign_y = Math::Sign(dy);
		auto dxFinal =
			sign_x * Math::Min(gridmap.GetGridTileWidth(), sign_x * dx);
		auto dyFinal =
			sign_y * Math::Min(gridmap.GetGridTileHeight(), sign_y * dy);

		gridmap.FilterGridMotion(position, &dxFinal, &dyFinal);
		position.x += dxFinal;
		position.y += dyFinal;

		if (!dxFinal)  // X motion denied
			dx = 0;
		else
			dx -= dxFinal;
		if (!dyFinal)  // Y motion denied
			dy = 0;
		else
			dy -= dyFinal;
	} while (dx || dy);
}

// TODO: fix y positioning
void GameEngine::CharacterController::AdjustToWalls(int x,
													int y,
													int offsetX,
													int offsetY,
													int width,
													int height) {
	auto original = position;
	auto changed =
		GameEngine::Graphics::Rect(x + offsetX, y + offsetY, width, height);

	if ((changed.x - original.x) < 0 && !original.IsEmpty()) {
		Move(-1, 0);
		if (position.Equals(original)) {
			position.x = original.x;
			position.width = width;
			position.height = height;
		} else {
			position.x = changed.x;
			position.width = width;
			position.height = height;
		}
		if (changed.y - original.y < 0) {
			Move(0, -1);
			if (position.y == original.y) {
				position.y = original.y;
			} else {
				position.y = changed.y;
			}
		} else {
			position.y = changed.y;
		}

	} else if (changed.x + changed.width - original.x - original.width > 0 &&
			   !original.IsEmpty()) {
		Move(1, 0);
		if (position.Equals(original)) {
			position.x = changed.x - std::abs(changed.x + changed.width -
											  original.x - original.width);
			position.width = changed.width;
			position.height = changed.height;
		} else {
			position.x = changed.x;
			position.width = width;
			position.height = height;
		}
		if (changed.y - original.y < 0) {
			Move(0, -1);
			if (position.y == original.y) {
				position.y = original.y;
			} else {
				position.y = changed.y;
			}
		} else {
			position.y = changed.y;
		}
	} else {
		if (changed.y - original.y < 0) {
			Move(0, -1);
			if (position.y == original.y) {
				position.y = original.y;
				position.height = original.height;
			} else {
				position.y = changed.y;
				position.height = height;
			}
		} else {
			position.y = changed.y;
			position.height = height;
		}
		position.x = changed.x;
		position.width = width;
	}
}
