#include <GameEngine/Graphics/Window.h>
#include <assert.h>

using namespace GameEngine::Graphics;

GameEngine::Graphics::Window::Window(std::string title,
									 int x,
									 int y,
									 int width,
									 int height,
									 Uint32 flags) {
	this->window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);

	if (!window) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Window creation failed: %s",
					 SDL_GetError());
		assert(window);
	}
}

bool GameEngine::Graphics::Window::IsOpen() const {
	if (!this->window)
		return false;
	Uint32 windowFlags = SDL_GetWindowFlags(this->window);
	return (windowFlags & SDL_WINDOW_SHOWN) &&
		   !(windowFlags & SDL_WINDOW_MINIMIZED) && this->isOpen;
}

void GameEngine::Graphics::Window::Close() {
	this->isOpen = false;
}

SDL_Window* GameEngine::Graphics::Window::GetWindow() const {
	return this->window;
}

void GameEngine::Graphics::Window::Destroy() {
	if (window) {
		SDL_DestroyWindow(this->GetWindow());
		this->window = nullptr;
	}
}
