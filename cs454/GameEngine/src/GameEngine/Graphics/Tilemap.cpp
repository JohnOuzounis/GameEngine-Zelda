#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/ImageLoader.h>
#include <GameEngine/Math.h>
#include <GameEngine/System.h>

#include<sstream>

using namespace GameEngine::Graphics;

GameEngine::Graphics::Tilemap::Tilemap(int w,
									   int h,
									   int tw,
									   int th,
									   std::string tilesetPath,
									   std::vector<int> map)
	: width(w), height(h), tileWidth(tw), tileHeight(th), map(map) {
	tileset = ImageLoader::GetImageLoader().Load(tilesetPath);
	view = Rect(0, 0, 0, 0);

	totalCols = width / tileWidth;
	totalRows = height / tileHeight;
	tilesetCols = tileset->GetWidth() / tw;
	tilesetRows = tileset->GetHeight() / th;

	tilemap = Image::Create(width, height, {0, 0, 0, 255});
	for (int row = 0; row < totalRows; row++) {
		for (int col = 0; col < totalCols; col++) {
			PutTile(row, col, GetTile(row, col));
		}
	}
}

GameEngine::Graphics::Tilemap::~Tilemap() {
	System::Destroy(tilemap);
	map.clear();
}

void GameEngine::Graphics::Tilemap::PutTile(int row, int col, int index) {
	int x = GetX(index), y = GetY(index);
	int xx = col * tileWidth, yy = row * tileHeight;

	tileset->Blit({x, y, tileWidth, tileHeight}, *tilemap,
				  {xx, yy, tileWidth, tileHeight});
}

bool GameEngine::Graphics::Tilemap::CanScrollHorizontal(float dx) const {
	return view.x >= -dx && (view.x + view.width + dx) <= tilemap->GetWidth();
}

bool GameEngine::Graphics::Tilemap::CanScrollVertical(float dy) const {
	return view.y >= -dy && (view.y + view.height + dy) <= tilemap->GetHeight();
}

void GameEngine::Graphics::Tilemap::Display(Image& dest,
											const Rect& displayArea) {
	if (view.x >= 0 && view.y >= 0 &&
		view.x + view.width <= tilemap->GetWidth() &&
		view.y + view.height <= tilemap->GetHeight()) {
		tilemap->BlitScaled(view, dest, displayArea);
	}
}

void GameEngine::Graphics::Tilemap::Scroll(int dx, int dy) {
	view.Move(dx, dy);

	view.x = Math::Clamp(view.x, 0, tilemap->GetWidth() - view.width - 1);
	view.y = Math::Clamp(view.y, 0, tilemap->GetHeight() - view.height - 1);
}