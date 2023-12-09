#pragma once

#include <SDL2/SDL.h>
#include <assert.h>

namespace GameEngine {
namespace Graphics {

class Event {
	typedef SDL_KeyboardEvent KeyboardEvent;
	typedef SDL_TextInputEvent TextInputEvent;
	typedef SDL_MouseButtonEvent MouseButtonEvent;
	typedef SDL_MouseMotionEvent MouseMotionEvent;
	typedef SDL_MouseWheelEvent MouseWheelEvent;
	typedef SDL_WindowEvent WindowEvent;

	typedef SDL_Keycode Key;

   public:
	enum Type {
		None = 0,
		Quit = SDL_QUIT,
		KeyDown = SDL_KEYDOWN,
		KeyUp = SDL_KEYUP,
		MouseMotion = SDL_MOUSEMOTION,
		MouseButtonDown = SDL_MOUSEBUTTONDOWN,
		MouseButtonUp = SDL_MOUSEBUTTONUP,
		MouseWheel = SDL_MOUSEWHEEL,
		TextInput = SDL_TEXTINPUT,
		Window = SDL_WINDOWEVENT
	};

	enum Keycode {
		Key_0 = SDLK_0,
		Key_1 = SDLK_1,
		Key_2 = SDLK_2,
		Key_3 = SDLK_3,
		Key_4 = SDLK_4,
		Key_5 = SDLK_5,
		Key_6 = SDLK_6,
		Key_7 = SDLK_7,
		Key_8 = SDLK_8,
		Key_9 = SDLK_9,
		Q = SDLK_q,
		W = SDLK_w,
		E = SDLK_e,
		R = SDLK_r,
		T = SDLK_t,
		Y = SDLK_y,
		U = SDLK_u,
		I = SDLK_i,
		O = SDLK_o,
		P = SDLK_p,
		A = SDLK_a,
		S = SDLK_s,
		D = SDLK_d,
		F = SDLK_f,
		G = SDLK_g,
		H = SDLK_h,
		J = SDLK_j,
		K = SDLK_k,
		L = SDLK_l,
		Z = SDLK_z,
		X = SDLK_x,
		C = SDLK_c,
		V = SDLK_v,
		B = SDLK_b,
		N = SDLK_n,
		M = SDLK_m,
		Space = SDLK_SPACE,
		LShift = SDLK_LSHIFT,
		RShift = SDLK_RSHIFT,
		Tab = SDLK_TAB,
		Ecsape = SDLK_ESCAPE,
		BackSpace = SDLK_BACKSPACE,
		Enter = SDLK_RETURN,
		Up = SDLK_UP,
		Down = SDLK_DOWN,
		Left = SDLK_LEFT,
		Right = SDLK_RIGHT,
		Home = SDLK_HOME,
		End = SDLK_END
	};

	private:
	Event() {}
	 Event(const Event&) = delete;
	 Event& operator=(const Event&) = delete;

	~Event() {}

	public:
	bool Poll();

	Type GetType() const;
	KeyboardEvent GetKey() const;
	WindowEvent GetWindow() const;
	TextInputEvent GetText() const;
	MouseWheelEvent GetWheel() const;
	MouseMotionEvent GetMotion() const;
	MouseButtonEvent GetButton() const;

   public:
	Key GetKeyPressed() const;
	static Event& GetEvent();

   private:
	SDL_Event event;
};

}  // namespace Graphics
}  // namespace GameEngine