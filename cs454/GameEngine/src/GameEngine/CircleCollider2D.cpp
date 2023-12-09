#include <GameEngine/BoxCollider2D.h>
#include <GameEngine/CircleCollider2D.h>
#include <GameEngine/Math.h>

bool GameEngine::CircleCollider2D::Overlap(const BoxCollider2D& box) const {

	int closestX =
		Math::Clamp(this->x, box.GetX(), box.GetX() + box.GetWidth());
	int closestY =
		Math::Clamp(this->y, box.GetY(), box.GetY() + box.GetHeight());

	Graphics::Point distance(this->x - closestX, this->y - closestY);
	return distance.Distance({0, 0}) <= this->radius;
}

bool GameEngine::CircleCollider2D::Overlap(
	const CircleCollider2D& circle) const {
	Graphics::Point center = {this->x, this->y};
	float distance = center.Distance({circle.GetX(), circle.GetY()});
	return distance <= this->radius + circle.GetRadius();
}

bool GameEngine::CircleCollider2D::Overlap(const Collider2D& collider) const {
	return collider.Overlap(*(Collider2D*)this);
}

bool GameEngine::CircleCollider2D::OverlapPoint(const int x,
												const int y) const {
	return this->OverlapPoint({x, y});
}

bool GameEngine::CircleCollider2D::OverlapPoint(
	const Graphics::Point& point) const {
	return point.Distance({this->x, this->y}) < radius;
}
