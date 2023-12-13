#pragma once
#include <GameEngine/Graphics/Color.h>
#include <GameEngine/Graphics/Image.h>
#include <unordered_set>

class TileColorsHolder final {
   private:
	std::unordered_set<int> indices;
	std::unordered_set<GameEngine::Graphics::Color::RGB> colors;

   public:
	void Insert(GameEngine::Graphics::Image& bmp, int index) {
		if (indices.find(index) == indices.end()) {
			indices.insert(index);
			bmp.AccessPixels([this, &bmp](Uint32 pixel) {
				GameEngine::Graphics::Color* color = bmp.GetPixelColor(pixel);
				colors.insert(color->hashRGB());
				delete color;
			});
		}
	}

	bool In(GameEngine::Graphics::Color c) const {
		return colors.find(c.hashRGB()) != colors.end();
	}
};

// keeps colors that are assumed to be empty
static TileColorsHolder emptyTileColors;

bool IsTileColorEmpty(GameEngine::Graphics::Color c) {
	return emptyTileColors.In(c);
}