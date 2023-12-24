#include <GameEngine/Graphics/Sprite.h>
#include <GameEngine/SpriteManager.h>

using namespace GameEngine;
using namespace GameEngine::Graphics;

Sprite::Sprite(int _x,
			   int _y,
			   AnimationFilm* film,
			   const std::string& _typeId)
	: x(_x), y(_y), currFilm(film), typeId(_typeId) {
	SetFrame(0);
	SpriteManager::GetSingleton().Add(this);
}

Sprite::~Sprite() {
	SpriteManager::GetSingleton().Remove(this);
}

bool GameEngine::Graphics::Sprite::CollisionCheck(const Sprite* s) const {
	return this->boundingArea->Overlap(*s->GetCollider());
}

void Sprite::Display(Image* dest,
					 const Rect& dpyArea,
					 const Clipper& clipper) const {
	Rect clippedBox;
	Point dpyPos;

	if (clipper.Clip(GetBox(), dpyArea, &dpyPos, &clippedBox)) {
		Rect clippedFrame{frameBox.x - clippedBox.x, frameBox.y - clippedBox.y,
						  clippedBox.width, clippedBox.height};
		currFilm->GetBitmap()->Blit(
			clippedFrame, *dest,
			{dpyPos.x, dpyPos.y, clippedFrame.width,
			 clippedFrame.height});
	}
}