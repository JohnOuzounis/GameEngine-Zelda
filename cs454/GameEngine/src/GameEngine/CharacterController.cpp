#include <GameEngine/CharacterController.h>
#include<GameEngine/Math.h>
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
