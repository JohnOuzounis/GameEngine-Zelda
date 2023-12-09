#pragma once

#include <SDL2/SDL.h>
#include <string>

namespace GameEngine {
namespace Graphics {

class Window {
   private:
	SDL_Window* window;
	bool isOpen = true;

   public:
	enum Position {
		Undefined = SDL_WINDOWPOS_UNDEFINED,
		Centered = SDL_WINDOWPOS_CENTERED
	};

	enum Flags {
		Shown = SDL_WINDOW_SHOWN,
		Hidden = SDL_WINDOW_HIDDEN,
		Minimized = SDL_WINDOW_MINIMIZED,
		Maximized = SDL_WINDOW_MAXIMIZED,
		Fullscreen = SDL_WINDOW_FULLSCREEN,
		Resizable = SDL_WINDOW_RESIZABLE
	};

   public:
	Window(std::string title, int width, int height, Uint32 flags)
		: Window(title,
				 (int)Position::Undefined,
				 (int)Position::Undefined,
				 width,
				 height,
				 flags) {}

	Window(std::string title,
		   int x,
		   int y,
		   int width,
		   int height,
		   Uint32 flags);

	~Window() { Destroy(); }

   public:
	bool IsOpen() const;
	SDL_Window* GetWindow() const;

	int GetWidth() const {
		int w, h;
		SDL_GetWindowSize(window, &w,&h);
		return w;
	}
	int GetHeight() const {
		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		return h;
	}

	/// <summary>
	/// Signal that the window should be closed, 
	/// it doesn't actually close the window
	/// </summary>
	void Close();

	void Destroy();
};
}  // namespace Graphics
}  // namespace GameEngine