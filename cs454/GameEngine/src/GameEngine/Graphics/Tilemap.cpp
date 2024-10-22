#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/ImageLoader.h>
#include <GameEngine/Math.h>
#include <GameEngine/System.h>

#include <sstream>

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
	bounds = Rect(0, 0, 0, 0);

	totalCols = width / tileWidth;
	totalRows = height / tileHeight;
	tilesetCols = tileset->GetWidth() / tw;
	tilesetRows = tileset->GetHeight() / th;

	tilemap = Image::Create(width, height, {123, 123, 123, 255});
	tilemap->SetColorKey({123, 123, 123}, true);
	for (int row = 0; row < totalRows; row++) {
		for (int col = 0; col < totalCols; col++) {
			PutTile(row, col, GetTile(row, col));
		}
	}
}

GameEngine::Graphics::Tilemap::~Tilemap() {
	System::Destroy(tilemap);
	System::Destroy(gridmap);
	map.clear();
}

void GameEngine::Graphics::Tilemap::PutTile(int row, int col, int index) {
	if (index < 0)	// empty tile
		return;

	int x = GetX(index), y = GetY(index);
	int xx = col * tileWidth, yy = row * tileHeight;

	tileset->Blit({x, y, tileWidth, tileHeight}, *tilemap,
				  {xx, yy, tileWidth, tileHeight});
}

bool GameEngine::Graphics::Tilemap::CanScrollHorizontal(int dx) const {
	return view.x >= -dx && (view.x + view.width + dx) <= tilemap->GetWidth();
}

bool GameEngine::Graphics::Tilemap::CanScrollVertical(int dy) const {
	return view.y >= -dy && (view.y + view.height + dy) <= tilemap->GetHeight();
}

void GameEngine::Graphics::Tilemap::Display(Image& dest,
											const Rect& displayArea) {
	tilemap->Blit(view, dest, displayArea);
}

void GameEngine::Graphics::Tilemap::Scroll(int dx, int dy) {
	view.Move(dx, dy);

	if (!bounds.IsEmpty()) {
		view.x = Math::Clamp(view.x, bounds.x,
							 bounds.x + bounds.width - view.width - 1);
		view.y = Math::Clamp(view.y, bounds.y,
							 bounds.y + bounds.height - view.height - 1);
	} else {
		view.x = Math::Clamp(view.x, 0, tilemap->GetWidth() - view.width - 1);
		view.y = Math::Clamp(view.y, 0, tilemap->GetHeight() - view.height - 1);
	}
}

void GameEngine::Graphics::Tilemap::SetGridmap(int gridTileWidth,
											   int gridTileHeight) {
	gridmap = new Gridmap(totalRows, totalCols, tileWidth, tileHeight,
						  gridTileWidth, gridTileHeight);
}

void GameEngine::Graphics::Tilemap::SetBounds(const Rect& bounds) {
	assert(bounds.width >= view.width && bounds.height >= view.height);
	assert(bounds.width <= tilemap->GetWidth() && bounds.height <= tilemap->GetHeight());

	this->bounds = bounds;
}
