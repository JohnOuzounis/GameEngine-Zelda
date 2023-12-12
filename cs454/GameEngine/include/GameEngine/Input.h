#pragma once

#include <GameEngine/Graphics/Event.h>
#include <unordered_map>
#include <vector>

namespace GameEngine {

class Input {
   public:
	enum MouseButton {
		MouseLeft = SDL_BUTTON_LEFT,
		MouseMiddle = SDL_BUTTON_MIDDLE,
		MouseRight = SDL_BUTTON_RIGHT
	};

	typedef struct MouseState {
		int mouseX, mouseY;
	} MouseState;

   private:
	typedef struct InputEvent {
		Graphics::Event::Type type;
		SDL_Keycode key;
		MouseButton mouse;
	} InputEvent;

	static std::vector<InputEvent> events;
	static std::unordered_map<SDL_Keycode, bool> keyStates;
	static std::unordered_map<MouseButton, bool> mouseStates;
	static std::unordered_map<SDL_Keycode, int> keyHeld;

   public:
	static void Init();
	static void CleanUp();

	static void ClearEvents();
	static void HandleEvent();

	static bool GetKeyDown(Graphics::Event::Keycode key);
	static bool GetKeyUp(Graphics::Event::Keycode key);
	static bool GetKey(Graphics::Event::Keycode key);

	static bool GetMouseButtonDown(MouseButton key);
	static bool GetMouseButtonUp(MouseButton key);
	static bool GetMouse(MouseButton key);
	static MouseState GetMouseState();
};
}  // namespace GameEngine