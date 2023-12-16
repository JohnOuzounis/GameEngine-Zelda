#include <GameEngine/Clipper.h>
using namespace GameEngine;
using namespace GameEngine::Graphics;

template <class T>
bool clip_rect(T x,
			   T y,
			   T w,
			   T h,
			   T wx,
			   T wy,
			   T ww,
			   T wh,
			   T* cx,
			   T* cy,
			   T* cw,
			   T* ch) {
	*cw = T(std::min(wx + ww, x + w)) - (*cx = T(std::max(wx, x)));
	*ch = T(std::min(wy + wh, y + h)) - (*cy = T(std::max(wy, y)));
	return *cw > 0 && *ch > 0;
}
bool clip_rect(const GameEngine::Graphics::Rect& r,
			   const GameEngine::Graphics::Rect& area,
			   GameEngine::Graphics::Rect* result) {
	return clip_rect(r.x, r.y, r.width, r.height, area.x, area.y, area.width,
					 area.height, &result->x, &result->y, &result->width,
					 &result->height);
}

bool Clipper::Clip(const Rect& r,
				   const Rect& dpyArea,
				   Point* dpyPos,
				   Rect* clippedBox) const {
	Rect visibleArea;
	if (!clip_rect(r, view(), &visibleArea)) {
		clippedBox->width = clippedBox->height = 0;
		return false;
	} else {
		// clippedBox is in ‘r’ coordinates, sub-rectangle of the input
		// rectangle
		clippedBox->x = r.x - visibleArea.x;
		clippedBox->y = r.y - visibleArea.y;
		clippedBox->width = visibleArea.width;
		clippedBox->height = visibleArea.height;
		dpyPos->x = dpyArea.x + (visibleArea.x - view().x);
		dpyPos->y = dpyArea.y + (visibleArea.y - view().y);
		return true;
	}
}