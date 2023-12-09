#pragma once

#include <GameEngine/Collider.h>
#include <GameEngine/Graphics/Rect.h>

namespace GameEngine {

class CircleCollider2D : public Collider2D {
   private:
	int x, y;
	int radius;

   public:
	CircleCollider2D(int x, int y, int radius, std::string tag)
		: x(x), y(y), radius(radius) {
		this->tag = tag;
	}
	CircleCollider2D(const Graphics::Point& center,
					 int radius,
					 std::string tag)
		: CircleCollider2D(center.x, center.y, radius, tag) {}

   public:
	virtual bool Overlap(const BoxCollider2D& box) const override;
	virtual bool Overlap(const CircleCollider2D& circle) const override;

	virtual bool Overlap(const Collider2D& collider) const override;

	virtual bool OverlapPoint(const int x, const int y) const override;
	virtual bool OverlapPoint(const Graphics::Point& point) const override;

	virtual Collider2D* Clone() const override {
		return new CircleCollider2D(x, y, radius, tag);
	}

   public:
	int GetX() const { return x; }
	int GetY() const { return y; }
	int GetRadius() const { return radius; }

	void SetX(int x) { this->x = x; }
	void SetY(int y) { this->y = y; }
	void SetRadius(int radius) { this->radius = radius; }
};
}  // namespace GameEngine