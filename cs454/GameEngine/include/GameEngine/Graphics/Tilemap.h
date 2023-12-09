#pragma once
#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Rect.h>
#include <string>
#include <vector>

namespace GameEngine {
namespace Graphics {
class Tilemap {
   private:
	Rect view;
	Image* tileset = nullptr;
	Image* tilemap = nullptr;

	int totalRows = 0, totalCols = 0;
	int tilesetRows = 0, tilesetCols = 0;
	int tileWidth = 0, tileHeight = 0;
	int width = 0, height = 0;
	std::vector<int> map;

	int GetX(int index) const { return (index % tilesetCols) * tileWidth; }
	int GetY(int index) const { return (index / tilesetCols) * tileHeight; }

   public:
	Tilemap(int w,
			int h,
			int tw,
			int th,
			std::string tilesetPath,
			std::vector<int> map);
	~Tilemap();

	void SetTile(int row, int col, int index) {
		map[(size_t)row * totalCols + col] = index;
	}
	int GetTile(int row, int col) const {
		return map[(size_t)row * totalCols + col];
	}
	void PutTile(int row, int col, int index);

	Image* GetTilemap() const { return tilemap; }
	int GetTileWidth() const { return tileWidth; }
	int GetTileHeight() const { return tileHeight; }
	int GetRows() const { return totalRows; }
	int GetColumns() const { return totalCols; }

	const Rect& GetView() const { return view; }
	void SetView(const Rect& view) { this->view = view; }

	bool CanScrollHorizontal(float dx) const;
	bool CanScrollVertical(float dy) const;

	void Display(Image& dest, const Rect& displayArea);
	void Scroll(int dx, int dy);
};
}  // namespace Graphics
}  // namespace GameEngine