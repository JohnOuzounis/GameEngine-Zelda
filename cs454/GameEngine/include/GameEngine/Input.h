#pragma once

#include <GameEngine/Graphics/Event.h>
#include <map>

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
	static std::map<SDL_Keycode, bool> keyStates;
	static std::map<MouseButton, bool> mouseStates;
	static std::map<SDL_Keycode, int> keyHeld;

   public:
	static void Init();
	static void CleanUp();

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