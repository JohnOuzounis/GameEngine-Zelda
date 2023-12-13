#pragma once

#include <SDL2/SDL.h>
#include <cassert>
#include <functional>

namespace GameEngine {
namespace Graphics {

class Color {
   public:
	typedef unsigned char RGB;
	using PixelFormat = SDL_PixelFormat;

	RGB r, g, b, a;

	Color(RGB r, RGB g, RGB b, RGB a) : r(r), g(g), b(b), a(a) {}
	Color(RGB r, RGB g, RGB b) : Color(r, g, b, 255) {}
	Color() : Color(0, 0, 0, 255) {}

	Color(const Color& color) {
		this->r = color.r;
		this->g = color.g;
		this->b = color.b;
		this->a = color.a;
	}

	~Color() {}

	Uint32 MapRGB(PixelFormat* format) const {
		assert(format);
		return SDL_MapRGB(format, this->r, this->g, this->b);
	}

	Uint32 MapRGBA(PixelFormat* format) const {
		assert(format);
		return SDL_MapRGBA(format, this->r, this->g, this->b, this->a);
	}

	bool operator!=(Color c) const {
		return c.r != this->r || c.g != this->g || c.b != this->b ||
			   c.a != this->a;
	}

	RGB hashRGB() {
		return (RGB)std::hash<GameEngine::Graphics::Color::RGB>()(r) ^
			   (RGB)std::hash<GameEngine::Graphics::Color::RGB>()(g) ^
			   (RGB)std::hash<GameEngine::Graphics::Color::RGB>()(b);
	}
};

}  // namespace Graphics
}  // namespace GameEngine