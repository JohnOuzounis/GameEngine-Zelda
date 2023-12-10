#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <GameEngine/Graphics/Color.h>
#include <GameEngine/Graphics/Rect.h>
#include <string>
#include <functional>

namespace GameEngine {
namespace Graphics {

class Image {
   private:
	SDL_Surface* surface;

   public:
	enum BlendMode {
		None = SDL_BLENDMODE_NONE,
		Blend = SDL_BLENDMODE_BLEND,
		Add = SDL_BLENDMODE_ADD,
		Mod = SDL_BLENDMODE_MOD,
		Mul = SDL_BLENDMODE_MUL
	};

	void Load(std::string path);

	void Destroy();

	Color* GetColorAt(const Point& x);
	Color* GetPixelColor(const Uint32) const;
	int GetWidth() const { return this->surface->w; }
	int GetHeight() const { return this->surface->h; }
	Uint8 GetBitDepth() const { return this->surface->format->BitsPerPixel; }
	SDL_Surface* GetSurface() const { return this->surface; }

	Uint32 GetColorKey() const;
	void SetColorKey(const Color& color, bool enable);

	void SetBlendMode(BlendMode mode);

	void Blit(const Rect& from, Image& dest, const Rect& to);
	void BlitScaled(const Rect& from, Image& dest, const Rect& to);
	
	void BlitMasked(const Rect& from,
					Image& dest,
					const Rect& to,
					const Color& mask);
	void BlitMaskedScaled(const Rect& from,
						  Image& dest,
						  const Rect& to,
						  const Color& mask);

	void BlitTinted(const Rect& from,
					Image& dest,
					const Rect& to,
					float rmod, float gmod, float bmod, float amod);

	void BlitTransparent(const Rect& from, Image& dest, const Rect& to, float trasparency);

	void Clear(const Color& color);
	void Clear(const Rect& rect, const Color& color);

	void Scale(int width, int height);
	void AccessPixels(std::function<void(const Uint32)> access);

	Image* Copy() const {
		SDL_Surface* surface = SDL_ConvertSurface(this->surface, this->surface->format, 0);
		return new Image(surface);
	}
	
	static Image* Create(const int width, const int height, const Color& color);

	void Lock() { SDL_LockSurface(this->surface); }
	void Unlock() { SDL_UnlockSurface(this->surface); }

   public:
	Image(SDL_Surface* surface) : surface(surface) {}
	Image(std::string path);
	~Image() { Destroy(); }
};
}  // namespace Graphics
}  // namespace GameEngine