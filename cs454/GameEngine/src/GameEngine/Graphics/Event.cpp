#include <GameEngine/Graphics/Event.h>
#include <assert.h>

using namespace GameEngine::Graphics;

bool Event::Poll() {
	return SDL_PollEvent(&this->event) == 1;
}

Event::KeyboardEvent Event::GetKey() const {
	assert(this->GetType() == Event::KeyUp ||
		   this->GetType() == Event::KeyDown);
	return this->event.key;
}

Event::WindowEvent Event::GetWindow() const {
	assert(this->GetType() == Event::Window);
	return this->event.window;
}

Event::TextInputEvent Event::GetText() const {
	assert(this->GetType() == Event::TextInput);
	return this->event.text;
}

Event::MouseWheelEvent Event::GetWheel() const {
	assert(this->GetType() == Event::MouseWheel);
	return this->event.wheel;
}

Event::MouseButtonEvent Event::GetButton() const {
	assert(this->GetType() == Event::MouseButtonUp ||
		   this->GetType() == Event::MouseButtonDown);
	return this->event.button;
}

Event::MouseMotionEvent Event::GetMotion() const {
	assert(this->GetType() == Event::MouseMotion);
	return this->event.motion;
}

Event::Key GameEngine::Graphics::Event::GetKeyPressed() const {
	return Event::GetKey().keysym.sym;
}

Event& GameEngine::Graphics::Event::GetEvent() {
	static Event event;
	return event;
}

Event::Type Event::GetType() const {
	switch (this->event.type) {
		case SDL_QUIT:
			return Event::Quit;
		case SDL_KEYDOWN:
			return Event::KeyDown;
		case SDL_KEYUP:
			return Event::KeyUp;
		case SDL_MOUSEMOTION:
			return Event::MouseMotion;
		case SDL_MOUSEBUTTONDOWN:
			return Event::MouseButtonDown;
		case SDL_MOUSEBUTTONUP:
			return Event::MouseButtonUp;
		case SDL_MOUSEWHEEL:
			return Event::MouseWheel;
		case SDL_TEXTINPUT:
			return Event::TextInput;
		case SDL_WINDOWEVENT:
			return Event::Window;
		default:
			break;
	}
	return None;
}