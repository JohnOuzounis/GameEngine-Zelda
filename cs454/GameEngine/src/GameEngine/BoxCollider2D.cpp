#include <GameEngine/BoxCollider2D.h>
#include <GameEngine/CircleCollider2D.h>

bool GameEngine::BoxCollider2D::Overlap(const BoxCollider2D& box) const {
	return !(this->rect.y > box.GetY() + box.GetWidth() ||	 // box is above me
		this->rect.y + this->rect.height < box.GetY() ||	// box is below me
		this->rect.x > box.GetX() + box.GetWidth() ||	// box is left of me
		this->rect.x + this->rect.width < box.GetX()	// box is right of me
	);
}

bool GameEngine::BoxCollider2D::Overlap(const CircleCollider2D& circle) const {
	return circle.Overlap(*this);
}

bool GameEngine::BoxCollider2D::Overlap(const Collider2D& collider) const {
	return collider.Overlap(*(Collider2D*)this);
}

bool GameEngine::BoxCollider2D::OverlapPoint(const int x, const int y) const {
	return (this->rect.x <= x && x <= this->rect.x + this->rect.width) &&
		   (this->rect.y <= y && y <= this->rect.y + this->rect.height);
}

bool GameEngine::BoxCollider2D::OverlapPoint(
	const Graphics::Point& point) const {
	return this->OverlapPoint(point.x, point.y);
}
