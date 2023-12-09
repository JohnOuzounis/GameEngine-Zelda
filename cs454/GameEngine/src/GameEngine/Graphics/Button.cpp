#include <GameEngine/Graphics/Button.h>
#include <GameEngine/Input.h>
#include <GameEngine/System.h>

GameEngine::Graphics::Button::~Button() {
	for (auto i : images) {
		System::Destroy(i);
	}
	images.clear();

	System::Destroy(text);
}

void GameEngine::Graphics::Button::HandleEvent() {
	Input::MouseState ms;

	auto contains = [&](Input::MouseState ms) {
		return ms.mouseX >= position.x &&
			   ms.mouseX <= position.width + position.x &&
			   ms.mouseY >= position.y &&
			   ms.mouseY <= position.height + position.y;
		;
	};

	ms = Input::GetMouseState();
	if (contains(ms)) {
		state = Hovered;
		OnHover();
	} else {
		state = Default;
	}

	if (Input::GetMouseButtonDown(Input::MouseLeft)) {
		ms = Input::GetMouseState();
		if (contains(ms)) {
			state = Clicked;
			OnClick();
		}
	} else if (Input::GetMouseButtonUp(Input::MouseLeft)) {
		ms = Input::GetMouseState();
		if (contains(ms)) {
			state = Hovered;
			OnRelease();
		}
	}
}
