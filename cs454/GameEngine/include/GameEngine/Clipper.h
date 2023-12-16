#pragma once
#include <GameEngine/Graphics/Rect.h>
#include <functional>

namespace GameEngine {

// generic clipper assuming any terrain-based view
// and any bitmap-based display area
class Clipper {
   public:
	using View = std::function<const Graphics::Rect&(void)>;

   private:
	View view;

   public:
	Clipper& SetView(const View& f) {
		view = f;
		return *this;
	}
	bool Clip(const Graphics::Rect& r,
			  const Graphics::Rect& dpyArea,
			  Graphics::Point* dpyPos,
			  Graphics::Rect* clippedBox) const;
	Clipper(void) = default;
	Clipper(const Clipper&) = default;
};

}  // namespace GameEngine