#include <GameEngine/Graphics/Gridmap.h>
#include <GameEngine/System.h>
#include <cassert>
#include <string>

void GameEngine::Graphics::Gridmap::SetSolidGridTile(int row, int col) {
	SetGridTile(row, col, GRID_SOLID_TILE);
}

void GameEngine::Graphics::Gridmap::SetEmptyGridTile(int row, int col) {
	SetGridTile(row, col, GRID_EMPTY_TILE);
}

void GameEngine::Graphics::Gridmap::SetGridTileFlags(int row,
													 int col,
													 GridIndex flags) {
	SetGridTile(row, col, flags);
}

void GameEngine::Graphics::Gridmap::SetGridTileTopSolidOnly(int row, int col) {
	SetGridTile(row, col, GRID_TOP_SOLID_MASK);
}

void GameEngine::Graphics::Gridmap::SetGridTileBlock(int row,
													 int col,
													 GridIndex flags) {
	for (int i = 0; i < gridBlockRows; ++i) {
		for (int j = 0; j < gridBlockColumns; ++j) {
			SetGridTile(row + i, col + j, flags);
		}
	}
}

void GameEngine::Graphics::Gridmap::SetGridTileBlock(int startRow,
													 int endRow,
													 int startCol,
													 int endCol,
													 GridIndex flags) {
	for (int i = startRow; i <= endRow; ++i) {
		for (int j = startCol; j <= endCol; ++j) {
			SetGridTile(i, j, flags);
		}
	}
}

bool GameEngine::Graphics::Gridmap::CanPassGridTile(int row,
													int col,
													GridIndex flags) const {
	return (GetGridTile(row, col) & flags) == 0;
}

GameEngine::Graphics::Gridmap::Gridmap(int rows,
									   int columns,
									   int tileWidth,
									   int tileHeight,
									   int gridTileWidth,
									   int gridTileHeight)
	: gridTileHeight(gridTileHeight), gridTileWidth(gridTileWidth) {
	gridBlockColumns = tileWidth / gridTileWidth;
	gridBlockRows = tileHeight / gridTileHeight;
	gridTilesPerTile = gridBlockColumns * gridBlockRows;

	totalColumns = gridBlockColumns * columns;
	totalRows = gridBlockRows * rows;

	grid = new GridIndex[total = totalColumns * totalRows];
	memset(grid, GRID_EMPTY_TILE, total);
}

GameEngine::Graphics::Gridmap::~Gridmap() {
	System::Destroy(grid);
}

void GameEngine::Graphics::Gridmap::FilterGridMotionLeft(const Rect& rect,
														 int* dx) const {
	auto x1_next = rect.x + *dx;
	if (x1_next < 0)
		*dx = -rect.x;
	else {
		auto newCol = x1_next / gridTileWidth;
		auto currCol = rect.x / gridTileWidth;
		if (newCol != currCol) {
			assert(newCol + 1 == currCol);	// we really move left
			auto startRow = rect.y / gridTileHeight;
			auto endRow = (rect.y + rect.height - 1) / gridTileHeight;
			for (auto row = startRow; row <= endRow; ++row)
				if (!CanPassGridTile(row, newCol, GRID_RIGHT_SOLID_MASK)) {
					*dx = currCol * gridTileWidth - rect.x;
					break;
				}
		}
	}
}

void GameEngine::Graphics::Gridmap::FilterGridMotionRight(const Rect& rect,
														  int* dx) const {
	auto x2 = rect.x + rect.width - 1;
	auto x2_next = x2 + *dx;
	if (x2_next >= totalColumns * gridTileWidth)
		*dx = (totalColumns * gridTileWidth - 1) - x2;
	else {
		auto newCol = x2_next / gridTileWidth;
		auto currCol = x2 / gridTileWidth;
		if (newCol != currCol) {
			assert(newCol - 1 == currCol);	// we really move right
			auto startRow = rect.y / gridTileHeight;
			auto endRow = (rect.y + rect.height - 1) / gridTileHeight;
			for (auto row = startRow; row <= endRow; ++row)
				if (!CanPassGridTile(row, newCol, GRID_LEFT_SOLID_MASK)) {
					*dx = (gridTileWidth * newCol - 1) - x2;
					break;
				}
		}
	}
}

void GameEngine::Graphics::Gridmap::FilterGridMotionUp(const Rect& rect,
													   int* dy) const {
	auto y1_next = rect.y + *dy;
	if (y1_next < 0)
		*dy = -rect.y;
	else {
		auto newRow = y1_next / gridTileHeight;
		auto currRow = rect.y / gridTileHeight;
		if (newRow != currRow) {
			assert(newRow + 1 == currRow);	// we really move up
			auto startCol = rect.x / gridTileWidth;
			auto endCol = (rect.x + rect.width - 1) / gridTileWidth;
			for (auto col = startCol; col <= endCol; ++col)
				if (!CanPassGridTile(newRow, col, GRID_BOTTOM_SOLID_MASK)) {
					*dy = currRow * gridTileHeight - rect.y;
					break;
				}
		}
	}
}

void GameEngine::Graphics::Gridmap::FilterGridMotionDown(const Rect& rect,
														 int* dy) const {
	auto y2 = rect.y + rect.height - 1;
	auto y2_next = y2 + *dy;
	if (y2_next >= totalRows * gridTileHeight)
		*dy = (totalRows * gridTileHeight - 1) - y2;
	else {
		auto newRow = y2_next / gridTileHeight;
		auto currRow = y2 / gridTileHeight;
		if (newRow != currRow) {
			assert(newRow - 1 == currRow);	// we really move right
			auto startCol = rect.x / gridTileWidth;
			auto endCol = (rect.x + rect.width - 1) / gridTileWidth;
			for (auto col = startCol; col <= endCol; ++col)
				if (!CanPassGridTile(newRow, col, GRID_TOP_SOLID_MASK)) {
					*dy = (gridTileHeight * newRow - 1) - y2;
					break;
				}
		}
	}
}

void GameEngine::Graphics::Gridmap::FilterGridMotion(const Rect& rect,
													 int* dx,
													 int* dy) const {
	assert(abs(*dx) <= gridTileWidth && abs(*dy) <= gridTileHeight);

	// try horizontal move
	if (*dx < 0)
		FilterGridMotionLeft(rect, dx);
	else if (*dx > 0)
		FilterGridMotionRight(rect, dx);

	// try vertical move
	if (*dy < 0)
		FilterGridMotionUp(rect, dy);
	else if (*dy > 0)
		FilterGridMotionDown(rect, dy);
}

bool GameEngine::Graphics::Gridmap::IsOnSolidGround(const Rect& rect) const {
	int dy = 1;	 // down 1 pixel
	FilterGridMotionDown(rect, &dy);
	return dy == 0;	 // if true IS attached to solid ground
}

void GameEngine::Graphics::Gridmap::SetGridTile(int row,
												int col,
												GridIndex index) {
	grid[row * totalColumns + col] = index;
}

GameEngine::Graphics::Gridmap::GridIndex
GameEngine::Graphics::Gridmap::GetGridTile(int row, int col) const {
	return grid[row * totalColumns + col];
}

GameEngine::Graphics::Gridmap::GridIndex*
GameEngine::Graphics::Gridmap::GetGridTileBlock(int row, int col) const {
	return grid + (row * totalColumns + col) * gridTilesPerTile;
}
