#pragma once

#include <GameEngine/Graphics/Image.h>
#include <SDL2/SDL.h>

namespace GameEngine {
namespace Graphics {

class Texture {
   public:
	   enum BlendMode {
		None = SDL_BLENDMODE_NONE,
		Blend = SDL_BLENDMODE_BLEND,
		Add = SDL_BLENDMODE_ADD,
		Mod = SDL_BLENDMODE_MOD,
		Mul = SDL_BLENDMODE_MUL
	};

   public:
	Texture(SDL_Renderer* renderer, const Image& image) {
		assert(renderer);
		CreateFromImage(renderer, image);
	}

	Texture(SDL_Renderer* renderer, int width, int height) {
		assert(renderer);
		this->texture =
			SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
							  SDL_TEXTUREACCESS_TARGET, width, height);
		if (!texture) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
						 "Texture creation failed: %s", SDL_GetError());
			assert(texture);
		}
	}

	~Texture() { Destroy(); }

   public:
	SDL_Texture* GetTexture() const { return texture; }

	void SetBlendMode(BlendMode mode) {
		SDL_SetTextureBlendMode(texture, (SDL_BlendMode)mode);
	}

	void Destroy() {
		if (texture) {
			SDL_DestroyTexture(texture);
			texture = nullptr;
		}
	}

   private:
	void CreateFromImage(SDL_Renderer* renderer, const Image& image) {
		assert(renderer);
		texture = SDL_CreateTextureFromSurface(renderer, image.GetSurface());
		if (!texture) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
						 "Texture creation failed: %s", SDL_GetError());
			assert(texture);
		}
	}

   private:
	SDL_Texture* texture;
};

}  // namespace Graphics
}  // namespace GameEngine