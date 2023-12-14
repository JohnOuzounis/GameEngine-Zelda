#pragma once
#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Math.h>

namespace GameEngine {
namespace Graphics {

class CircularBackground {	// horizontal stripe
   private:
	Rect viewWin;
	Image* bg = nullptr;

   public:
	CircularBackground(Image* bg) : bg(bg) {}

	Image* GetBackGround() const { return bg; }
	const Rect& GetView() const { return viewWin; }
	void SetView(const Rect& view) { viewWin = view; }

	void Scroll(int dx) {
		viewWin.x += dx;
		if (viewWin.x < 0)
			viewWin.x = bg->GetWidth() + viewWin.x;
		else if (viewWin.x >= bg->GetWidth())
			viewWin.x = viewWin.x - bg->GetWidth();
	}

	void Display(Image* dest, int x, int y) const {
		auto bg_w = bg->GetWidth();
		auto w1 = Math::Min(bg_w - viewWin.x, viewWin.width);
		bg->Blit({viewWin.x, viewWin.y, w1, viewWin.height}, *dest,
				 {x, y, viewWin.width, viewWin.height});
		if (w1 < viewWin.width) {		   // not whole view win fits
			auto w2 = viewWin.width - w1;  // the remaining part
			bg->Blit({0, viewWin.y, w2, viewWin.height}, *dest,
					 {x + w1, y, viewWin.width, viewWin.height});
		}
	}
};

}  // namespace Graphics
}  // namespace GameEngine