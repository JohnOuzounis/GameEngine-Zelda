#pragma once

#include <GameEngine/Graphics/Rect.h>
#include <string>

namespace GameEngine {

class BoxCollider2D;
class CircleCollider2D;

class Collider2D {
   protected:
	friend class BoxCollider2D;
	friend class CircleCollider2D; 

	std::string tag;
	bool isTrigger = false;

	virtual bool Overlap(const BoxCollider2D& box) const = 0;
	virtual bool Overlap(const CircleCollider2D& circle) const = 0;

   public:
	virtual ~Collider2D() {}

   public:
	virtual bool Overlap(const Collider2D& collider) const = 0;

	virtual bool OverlapPoint(const int x, const int y) const = 0;
	virtual bool OverlapPoint(const Graphics::Point& point) const = 0;

	virtual Collider2D* Clone() const = 0;
	std::string GetTag() const { return tag; }

	bool IsTrigger() const { return isTrigger; }
	void SetTrigger(bool isTrigger) { this->isTrigger = isTrigger; }
};

}  // namespace GameEngine