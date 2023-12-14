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

	void Display(Image* dest, const Rect& displayArea) const {
		Image* temp =
			Image::Create(viewWin.width, viewWin.height, {123, 123, 123, 255});
		temp->SetColorKey({123, 123, 123}, true);

		auto bg_w = bg->GetWidth();
		auto w1 = Math::Min(bg_w - viewWin.x, viewWin.width);
		bg->BlitScaled({viewWin.x, viewWin.y, w1, viewWin.height}, *temp,
					   {0, 0, w1, viewWin.height});
		if (w1 < viewWin.width) {		   // not whole view win fits
			auto w2 = viewWin.width - w1;  // the remaining part
			bg->BlitScaled({0, viewWin.y, w2, viewWin.height}, *temp,
						   {w1, 0, w2, viewWin.height});
		}

		temp->BlitScaled({0, 0, temp->GetWidth(), temp->GetHeight()}, *dest,
						 {displayArea.x, displayArea.y, displayArea.width,
						  displayArea.height});
	}
};

}  // namespace Graphics
}  // namespace GameEngine