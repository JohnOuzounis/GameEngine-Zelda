#include <GameEngine/Animation/AnimationFilm.h>

GameEngine::AnimationFilm::AnimationFilm(
	Graphics::Image* b,
	const std::vector<Graphics::Rect>& boxes,
	const std::string& id) {

	this->SetBitmap(b);
	this->id = id;
	this->boxes = boxes;
}
