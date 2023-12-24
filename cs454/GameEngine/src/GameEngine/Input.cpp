#include <GameEngine/Input.h>

using namespace GameEngine;
using namespace GameEngine::Graphics;

std::unordered_map<SDL_Keycode, bool> Input::keyStates;
std::unordered_map<Input::MouseButton, bool> Input::mouseStates;
std::unordered_map<SDL_Keycode, int> Input::keyHeld;
std::vector<Input::InputEvent> Input::events;

void GameEngine::Input::Init() {
	mouseStates[MouseButton::MouseLeft] = false;
	mouseStates[MouseButton::MouseMiddle] = false;
	mouseStates[MouseButton::MouseRight] = false;

	keyStates[Event::Key_0] = false;
	keyStates[Event::Key_1] = false;
	keyStates[Event::Key_2] = false;
	keyStates[Event::Key_3] = false;
	keyStates[Event::Key_4] = false;
	keyStates[Event::Key_5] = false;
	keyStates[Event::Key_6] = false;
	keyStates[Event::Key_7] = false;
	keyStates[Event::Key_8] = false;
	keyStates[Event::Key_9] = false;
	keyStates[Event::Q] = false;
	keyStates[Event::W] = false;
	keyStates[Event::E] = false;
	keyStates[Event::R] = false;
	keyStates[Event::T] = false;
	keyStates[Event::Y] = false;
	keyStates[Event::U] = false;
	keyStates[Event::I] = false;
	keyStates[Event::O] = false;
	keyStates[Event::P] = false;
	keyStates[Event::A] = false;
	keyStates[Event::S] = false;
	keyStates[Event::D] = false;
	keyStates[Event::F] = false;
	keyStates[Event::G] = false;
	keyStates[Event::H] = false;
	keyStates[Event::J] = false;
	keyStates[Event::K] = false;
	keyStates[Event::L] = false;
	keyStates[Event::Z] = false;
	keyStates[Event::X] = false;
	keyStates[Event::C] = false;
	keyStates[Event::V] = false;
	keyStates[Event::B] = false;
	keyStates[Event::N] = false;
	keyStates[Event::M] = false;
	keyStates[Event::Space] = false;
	keyStates[Event::LShift] = false;
	keyStates[Event::RShift] = false;
	keyStates[Event::Tab] = false;
	keyStates[Event::Ecsape] = false;
	keyStates[Event::BackSpace] = false;
	keyStates[Event::Enter] = false;
	keyStates[Event::Up] = false;
	keyStates[Event::Down] = false;
	keyStates[Event::Left] = false;
	keyStates[Event::Right] = false;
	keyStates[Event::Home] = false;
	keyStates[Event::End] = false;
	keyStates[Event::Plus] = false;
	keyStates[Event::Minus] = false;

	keyHeld[Event::Key_0] = 0;
	keyHeld[Event::Key_1] = 0;
	keyHeld[Event::Key_2] = 0;
	keyHeld[Event::Key_3] = 0;
	keyHeld[Event::Key_4] = 0;
	keyHeld[Event::Key_5] = 0;
	keyHeld[Event::Key_6] = 0;
	keyHeld[Event::Key_7] = 0;
	keyHeld[Event::Key_8] = 0;
	keyHeld[Event::Key_9] = 0;
	keyHeld[Event::Q] = 0;
	keyHeld[Event::W] = 0;
	keyHeld[Event::E] = 0;
	keyHeld[Event::R] = 0;
	keyHeld[Event::T] = 0;
	keyHeld[Event::Y] = 0;
	keyHeld[Event::U] = 0;
	keyHeld[Event::I] = 0;
	keyHeld[Event::O] = 0;
	keyHeld[Event::P] = 0;
	keyHeld[Event::A] = 0;
	keyHeld[Event::S] = 0;
	keyHeld[Event::D] = 0;
	keyHeld[Event::F] = 0;
	keyHeld[Event::G] = 0;
	keyHeld[Event::H] = 0;
	keyHeld[Event::J] = 0;
	keyHeld[Event::K] = 0;
	keyHeld[Event::L] = 0;
	keyHeld[Event::Z] = 0;
	keyHeld[Event::X] = 0;
	keyHeld[Event::C] = 0;
	keyHeld[Event::V] = 0;
	keyHeld[Event::B] = 0;
	keyHeld[Event::N] = 0;
	keyHeld[Event::M] = 0;
	keyHeld[Event::Space] = 0;
	keyHeld[Event::LShift] = 0;
	keyHeld[Event::RShift] = 0;
	keyHeld[Event::Tab] = 0;
	keyHeld[Event::Ecsape] = 0;
	keyHeld[Event::BackSpace] = 0;
	keyHeld[Event::Enter] = 0;
	keyHeld[Event::Up] = 0;
	keyHeld[Event::Down] = 0;
	keyHeld[Event::Left] = 0;
	keyHeld[Event::Right] = 0;
	keyHeld[Event::Home] = 0;
	keyHeld[Event::End] = 0;
	keyHeld[Event::Plus] = 0;
	keyHeld[Event::Minus] = 0;
}

void GameEngine::Input::CleanUp() {
	keyStates.clear();
	keyHeld.clear();
	mouseStates.clear();
	events.clear();
}

void Input::ClearEvents() {
	events.clear();
}

void GameEngine::Input::HandleEvent() {
	InputEvent event;

	if (Event::GetEvent().GetType() == Event::KeyDown) {
		event.type = Event::KeyDown;
		event.key = Event::GetEvent().GetKeyPressed();
		events.push_back(event);

		keyStates[Event::GetEvent().GetKeyPressed()] = true;
		keyHeld[Event::GetEvent().GetKeyPressed()] =
			(keyHeld[Event::GetEvent().GetKeyPressed()] > 3)
				? 3
				: keyHeld[Event::GetEvent().GetKeyPressed()] + 1;
	}
	if (Event::GetEvent().GetType() == Event::KeyUp) {
		event.type = Event::KeyUp;
		event.key = Event::GetEvent().GetKeyPressed();
		events.push_back(event);

		keyStates[Event::GetEvent().GetKeyPressed()] = false;
		keyHeld[Event::GetEvent().GetKeyPressed()] = 0;
		assert(keyHeld[Event::GetEvent().GetKeyPressed()] >= 0);
	}

	if (Event::GetEvent().GetType() == Event::MouseButtonDown)
		if (Event::GetEvent().GetButton().button <= 3) {
			event.type = Event::MouseButtonDown;
			event.mouse = (MouseButton)Event::GetEvent().GetButton().button;
			events.push_back(event);

			mouseStates[(MouseButton)Event::GetEvent().GetButton().button] =
				true;
		}

	if (Event::GetEvent().GetType() == Event::MouseButtonUp)
		if (Event::GetEvent().GetButton().button <= 3) {
			event.type = Event::MouseButtonUp;
			event.mouse = (MouseButton)Event::GetEvent().GetButton().button;
			events.push_back(event);

			mouseStates[(MouseButton)Event::GetEvent().GetButton().button] =
				false;
		}
}

bool GameEngine::Input::GetKeyDown(Graphics::Event::Keycode key) {
	for (auto& event : events) {
		if (event.type == Event::KeyDown && keyStates[key] && event.key == key)
			return true;
	}
	return false;
}

bool GameEngine::Input::GetKeyUp(Graphics::Event::Keycode key) {
	for (auto& event : events) {
		if (event.type == Event::KeyUp && !keyStates[key] && event.key == key)
			return true;
	}
	return false;
}

bool GameEngine::Input::GetKey(Graphics::Event::Keycode key) {
	return Input::GetKeyDown(key) && keyHeld[key] >= 2;
}

bool GameEngine::Input::GetMouseButtonDown(MouseButton key) {
	for (auto& event : events) {
		if (event.type == Event::MouseButtonDown && mouseStates[key] &&
			event.mouse == key)
			return true;
	}
	return false;
}

bool GameEngine::Input::GetMouseButtonUp(MouseButton key) {
	for (auto& event : events) {
		if (event.type == Event::MouseButtonUp && !mouseStates[key] &&
			event.mouse == key)
			return true;
	}
	return false;
}

bool GameEngine::Input::GetMouse(MouseButton key) {
	return mouseStates[key];
}

Input::MouseState GameEngine::Input::GetMouseState() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	return {x, y};
}
