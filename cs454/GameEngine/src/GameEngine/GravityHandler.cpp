#include <GameEngine/GravityHandler.h>
using namespace GameEngine;
using namespace GameEngine::Graphics;

void GravityHandler::Check(const Rect& r) {
	if (gravityAddicted) {
		if (onSolidGround(r)) {
			if (isFalling) {
				isFalling = false;
				onStopFalling();
			}
		} else if (!isFalling) {
			isFalling = true;
			onStartFalling();
		}
	}
}