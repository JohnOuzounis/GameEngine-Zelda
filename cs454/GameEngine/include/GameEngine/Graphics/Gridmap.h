#pragma once
#include <GameEngine/Graphics/Rect.h>

#define GRID_THIN_AIR_MASK 0x0000	   // element is ignored
#define GRID_LEFT_SOLID_MASK 0x0001	   // bit 0
#define GRID_RIGHT_SOLID_MASK 0x0002   // bit 1
#define GRID_TOP_SOLID_MASK 0x0004	   // bit 2
#define GRID_BOTTOM_SOLID_MASK 0x0008  // bit 3
#define GRID_GROUND_MASK 0x0010	 // bit 4, keep objects top / bottom (gravity)
#define GRID_FLOATING_MASK \
	0x0020	// bit 5, keep objects anywhere inside (gravity)

#define GRID_EMPTY_TILE GRID_THIN_AIR_MASK
#define GRID_SOLID_TILE                                                   \
	(GRID_LEFT_SOLID_MASK | GRID_RIGHT_SOLID_MASK | GRID_TOP_SOLID_MASK | \
	 GRID_BOTTOM_SOLID_MASK)

namespace GameEngine {
namespace Graphics {

class Gridmap {
   public:
	typedef unsigned char GridIndex;

   private:
	GridIndex* grid = nullptr;
	int total = 0;
	int totalRows = 0, totalColumns = 0;
	int gridTileWidth = 0, gridTileHeight = 0;
	int gridBlockRows = 0, gridBlockColumns = 0;
	int gridTilesPerTile = 0;

	void FilterGridMotionLeft(const Rect& rect, int* dx) const;
	void FilterGridMotionRight(const Rect& rect, int* dx) const;
	void FilterGridMotionUp(const Rect& rect, int* dy) const;
	void FilterGridMotionDown(const Rect& rect, int* dy) const;

   public:
	void FilterGridMotion(const Rect& rect, int* dx, int* dy) const;
	bool IsOnSolidGround(const Rect& rect) const;

	void SetGridTile(int row, int col, GridIndex index);
	GridIndex GetGridTile(int row, int col) const;
	GridIndex* GetGridTileBlock(int row, int col) const;

	GridIndex* GetGrid() const { return grid; }
	int GetTotal() const { return total; }
	int GetGridTileWidth() const { return gridTileWidth; }
	int GetGridTileHeight() const { return gridTileHeight; }
	int GetGridBlockRows() const { return gridBlockRows; }
	int GetGridBlockColumns() const { return gridBlockColumns; }
	int GetGridTilesPerTile() const { return gridTilesPerTile; }
	int GetGridRows() const { return totalRows; }
	int GetGridColumns() const { return totalColumns; }

	void SetSolidGridTile(int row, int col);
	void SetEmptyGridTile(int row, int col);
	void SetGridTileFlags(int row, int col, GridIndex flags);
	void SetGridTileTopSolidOnly(int row, int col);
	void SetGridTileBlock(int row, int col, GridIndex flags);
	void SetGridTileBlock(int startRow, int endRow, int startCol, int endCol, GridIndex flags);

	bool CanPassGridTile(int row, int col, GridIndex flags) const;

	Gridmap(int rows,
			int columns,
			int tileWidth,
			int tileHeight,
			int gridTileWidth,
			int gridTileHeight);
	~Gridmap();
};
}  // namespace Graphics
}  // namespace GameEngine