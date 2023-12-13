#pragma once
#include <vector>
#include <GameEngine/Graphics/Color.h>
#include <GameEngine/Graphics/Image.h>

class TileColorsHolder final {
   private:
	std::vector<int> indices;
	std::vector<GameEngine::Graphics::Color> colors;

   public:
	void Insert(GameEngine::Graphics::Image& bmp, int index) {
		if (std::find(indices.begin(), indices.end(), index) == indices.end()) {
			indices.push_back(index);
			bmp.AccessPixels([this, &bmp](Uint32 pixel) {
				GameEngine::Graphics::Color* color = bmp.GetPixelColor(pixel);
				colors.push_back(*color);
				delete color;
			});
		}
	}
	bool In(GameEngine::Graphics::Color c) const {
		for (auto color : colors) {
			if (!(color != c))
				return true;
		}
		return false;
	}
};
// keeps colors that are assumed to be empty
static TileColorsHolder emptyTileColors;
bool IsTileColorEmpty(GameEngine::Graphics::Color c) {
	return emptyTileColors.In(c);
}  // return false to disable
