#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Math.h>
#include <GameEngine/System.h>
#include <assert.h>
#include <iostream>

using namespace GameEngine::Graphics;

void GameEngine::Graphics::Image::Load(std::string path) {
	this->Destroy();
	this->surface = IMG_Load(path.c_str());

	if (!surface) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Image loading failed: %s",
					 IMG_GetError());
		assert(surface);
	}
}

void GameEngine::Graphics::Image::Destroy() {
	if (surface) {
		SDL_FreeSurface(this->surface);
		surface = nullptr;
	}
}

Color* GameEngine::Graphics::Image::GetColorAt(const Point& x) {
	assert(surface);

	Uint32* pixels = static_cast<Uint32*>(this->surface->pixels);
	Uint32 pixel = pixels[x.y * this->surface->w + x.x];
	Uint8 red, green, blue, alpha;
	SDL_GetRGBA(pixel, this->surface->format, &red, &green, &blue, &alpha);
	return new Color(red, green, blue, alpha);
}

Color* GameEngine::Graphics::Image::GetPixelColor(const Uint32 pixel) const {
	Color* color = new Color();
	SDL_GetRGBA(pixel, this->surface->format, &color->r, &color->g, &color->b, &color->a);
	return color;
}

Uint32 GameEngine::Graphics::Image::GetColorKey() const {
	assert(surface);

	Uint32 key;
	SDL_GetColorKey(this->surface, &key);
	return key;
}

void GameEngine::Graphics::Image::SetColorKey(const Color& color, bool enable) {
	assert(surface);

	int flag = (enable) ? SDL_TRUE : SDL_FALSE;
	SDL_SetColorKey(this->surface, flag, color.MapRGB(this->surface->format));
}

void GameEngine::Graphics::Image::SetBlendMode(BlendMode mode) {
	SDL_SetSurfaceBlendMode(surface, (SDL_BlendMode)mode);
}

void GameEngine::Graphics::Image::Blit(const Rect& from,
									   Image& dest,
									   const Rect& to) {
	assert(surface);

	SDL_Rect src = from.GetRect();
	SDL_Rect dst = to.GetRect();
	SDL_BlitSurface(this->surface, (!from.IsEmpty()) ? &src : NULL,
					dest.GetSurface(), (!to.IsEmpty()) ? &dst : NULL);
}

void GameEngine::Graphics::Image::BlitScaled(const Rect& from,
											 Image& dest,
											 const Rect& to) {
	assert(surface);

	SDL_Rect src = from.GetRect();
	SDL_Rect dst = to.GetRect();
	SDL_BlitScaled(this->surface, (!from.IsEmpty()) ? &src : NULL,
					dest.GetSurface(), (!to.IsEmpty()) ? &dst : NULL);
}

void GameEngine::Graphics::Image::BlitMasked(const Rect& from,
											 Image& dest,
											 const Rect& to,
											 const Color& mask) {
	assert(surface);

	this->SetColorKey(mask, true);

	SDL_Rect src = from.GetRect();
	SDL_Rect dst = to.GetRect();
	SDL_BlitSurface(this->surface, (!from.IsEmpty()) ? &src : NULL,
					dest.GetSurface(), (!to.IsEmpty()) ? &dst : NULL);
}

void GameEngine::Graphics::Image::BlitMaskedScaled(const Rect& from,
												   Image& dest,
												   const Rect& to,
												   const Color& mask) {
	assert(surface);

	this->SetColorKey(mask, true);

	SDL_Rect src = from.GetRect();
	SDL_Rect dst = to.GetRect();
	SDL_BlitScaled(this->surface, (!from.IsEmpty()) ? &src : NULL,
					dest.GetSurface(), (!to.IsEmpty()) ? &dst : NULL);
}

void GameEngine::Graphics::Image::BlitTinted(const Rect& from,
											 Image& dest,
											 const Rect& to,
											 float rmod,
											 float gmod,
											 float bmod,
											 float amod) {
	assert(surface);
	Image* tinted = this->Copy();
	tinted->Lock();

	Uint32* pixels = static_cast<Uint32*>(tinted->GetSurface()->pixels);
	int pixelCount = tinted->GetWidth() * tinted->GetHeight();
	for (int i = 0; i < pixelCount; i++) {
		Uint8 r, g, b, a;
		SDL_GetRGBA(pixels[i], tinted->GetSurface()->format, &r, &g, &b, &a);
		r = static_cast<Uint8>(r * Math::Clamp(rmod, (float)0, (float)1));
		g = static_cast<Uint8>(g * Math::Clamp(gmod, (float)0, (float)1));
		b = static_cast<Uint8>(b * Math::Clamp(bmod, (float)0, (float)1));
		a = static_cast<Uint8>(a * Math::Clamp(amod, (float)0, (float)1));
		pixels[i] = SDL_MapRGBA(tinted->GetSurface()->format, r, g, b, a);
	}
	tinted->Unlock();
	tinted->Blit(from, dest, to);

	System::Destroy(tinted);
}

void GameEngine::Graphics::Image::BlitTransparent(const Rect& from,
												  Image& dest,
												  const Rect& to,
												  float trasparency) {
	this->BlitTinted(from, dest, to, 1, 1, 1, trasparency);
}

void GameEngine::Graphics::Image::Clear(const Color& color) {
	assert(surface);
	SDL_FillRect(this->surface, NULL, color.MapRGBA(this->surface->format));
}

void GameEngine::Graphics::Image::Clear(const Rect& rect, const Color& color) {
	assert(surface);
	SDL_Rect r = rect.GetRect();
	SDL_FillRect(this->surface, (!rect.IsEmpty()) ? &r : NULL,
				 color.MapRGBA(this->surface->format));
}

void GameEngine::Graphics::Image::Scale(int width, int height) {
	SDL_Surface* tmp = SDL_CreateRGBSurface(
	surface->flags, surface->w, surface->h, 32, surface->format->Rmask, surface->format->Gmask,
	surface->format->Bmask, surface->format->Amask);
	SDL_BlitSurface(surface, NULL, tmp, NULL);
	
	SDL_Rect target = {0, 0, width, height};
	
	SDL_Surface* tmp2 = SDL_CreateRGBSurface(
		tmp->flags, target.w, target.h, 32, tmp->format->Rmask,
		tmp->format->Gmask,
	tmp->format->Bmask, tmp->format->Amask);
	SDL_FillRect(tmp2, &target,
			 SDL_MapRGBA(tmp2->format, 255, 0, 0, 255));

	SDL_BlitScaled(tmp, NULL, tmp2, NULL);
	
	SDL_FreeSurface(surface);
	SDL_FreeSurface(tmp);

	surface = tmp2;
}

void GameEngine::Graphics::Image::AccessPixels(std::function<void(const Uint32)> access) {
	assert(surface);
	this->Lock();
	Uint32* pixels = static_cast<Uint32*>(this->GetSurface()->pixels);
	int pixelCount = this->GetWidth() * this->GetHeight();
	for (int i = 0; i < pixelCount; i++) {
		access(pixels[i]);
	}
	this->Unlock();
}

Image* GameEngine::Graphics::Image::Create(const int width,
										   const int height,
										   const Color& color) {
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface* surface = SDL_CreateRGBSurface(
		0, width, height, 32, rmask, gmask, bmask, amask);
	
	SDL_FillRect(surface, NULL, color.MapRGBA(surface->format));
	return new Image(surface);
}

GameEngine::Graphics::Image::Image(std::string path) {
	surface = nullptr;
	this->Load(path);
}
