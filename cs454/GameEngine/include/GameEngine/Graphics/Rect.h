#pragma once

#include <SDL2/SDL.h>
#include <cmath>

namespace GameEngine {
namespace Graphics {

class Point {
   public:
	int x = 0, y = 0;

	Point(int x, int y) : x(x), y(y) {}
	Point() {}

	~Point() {}

   public:
	float Distance(const Point& point) const {
		return std::sqrtf((float)(x - point.x) * (x - point.x) +
						  (float)(y - point.y) * (y - point.y));
	}
};

class Rect {
   public:
	int x = 0, y = 0;
	int width = 0, height = 0;

	Rect(int x, int y, int width, int height)
		: x(x), y(y), width(width), height(height) {}

	Rect(Point& point, int width, int height)
		: Rect(point.x, point.y, width, height) {}

	Rect(const Rect& rect) { this->Assign(rect); }

	Rect() : Rect(0, 0, 0, 0) {}

	~Rect() {}

   public:
	SDL_Rect GetRect() const { return {x, y, width, height}; }
	bool IsEmpty() const { return width == 0 || height == 0; }

	void Assign(const Rect& rect) {
		this->x = rect.x;
		this->y = rect.y;
		this->width = rect.width;
		this->height = rect.height;
	}

	void Move(int dx, int dy) {
		x += dx;
		y += dy;
	}

	void MoveTo(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

}  // namespace Graphics
}  // namespace GameEngine