#pragma once

#include <GameEngine/Collider.h>
#include <GameEngine/Graphics/Rect.h>

namespace GameEngine {

class BoxCollider2D : public Collider2D {
   private:
	Graphics::Rect rect;

   public:
	BoxCollider2D(int x, int y, int width, int height, std::string tag) {
		this->rect = Graphics::Rect(x, y, width, height);
		this->tag = tag;
	}
	BoxCollider2D(const Graphics::Point& point,
				  int width,
				  int height,
				  std::string tag)
		: BoxCollider2D(point.x, point.y, width, height, tag) {}

	~BoxCollider2D() { }

   public:
	virtual bool Overlap(const BoxCollider2D& box) const override;
	virtual bool Overlap(const CircleCollider2D& circle) const override;

	virtual bool Overlap(const Collider2D& collider) const override;

	virtual bool OverlapPoint(const int x, const int y) const override;
	virtual bool OverlapPoint(const Graphics::Point& point) const override;

	virtual Collider2D* Clone() const override {
		return new BoxCollider2D({rect.x, rect.y}, rect.width, rect.height,
								 tag);
	}

   public:
	int GetX() const { return this->rect.x; }
	int GetY() const { return this->rect.y; }
	int GetWidth() const { return this->rect.width; }
	int GetHeight() const { return this->rect.height; }
	const Graphics::Rect& GetRect() const { return this->rect; }

	void SetX(int x) { this->rect.x = x; }
	void SetY(int y) { this->rect.y = y; }
	void SetWidth(int width) { this->rect.width = width; }
	void SetHeight(int height) { this->rect.height = height; }
	void SetRect(const Graphics::Rect& rect) { this->rect.Assign(rect); }
};

}  // namespace GameEngine