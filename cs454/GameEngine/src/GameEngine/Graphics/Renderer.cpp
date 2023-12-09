#include <GameEngine/Graphics/Renderer.h>
#include <assert.h>

GameEngine::Graphics::Renderer::Renderer(Window& window,
										 int index,
										 Uint32 flags) {
	this->renderer = SDL_CreateRenderer(window.GetWindow(), index, flags);
	if (!this->renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
					 "Renderer creation failed: %s", SDL_GetError());
		assert(renderer);
	}
}

SDL_Renderer* GameEngine::Graphics::Renderer::GetRenderer() const {
	return this->renderer;
}

bool GameEngine::Graphics::Renderer::Clear() const {
	assert(renderer);
	return SDL_RenderClear(this->renderer) == 0;
}

bool GameEngine::Graphics::Renderer::Copy(Texture& texture,
										  const Rect& src,
										  const Rect& dest) const {
	assert(renderer);

	SDL_Rect s = src.GetRect();
	SDL_Rect d = dest.GetRect();

	return SDL_RenderCopy(this->renderer, texture.GetTexture(),
						  (src.IsEmpty()) ? NULL : &s,
						  (dest.IsEmpty()) ? NULL : &d);
}

void GameEngine::Graphics::Renderer::Render() const {
	assert(renderer);
	SDL_RenderPresent(this->renderer);
}

bool GameEngine::Graphics::Renderer::SetDrawColor(const Color& color) {
	assert(renderer);
	return SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b,
								  color.a) == 0;
}

bool GameEngine::Graphics::Renderer::SetTarget(Texture* texture) {
	return SDL_SetRenderTarget(
			   this->renderer,
			   (texture == nullptr) ? NULL : texture->GetTexture()) == 0;
}

bool GameEngine::Graphics::Renderer::DrawPoint(const Point& point) const {
	assert(renderer);

	return SDL_RenderDrawPoint(this->renderer, point.x, point.y) == 0;
}

bool GameEngine::Graphics::Renderer::DrawLine(const Point& point1,
											  const Point& point2) const {
	assert(renderer);

	return SDL_RenderDrawLine(this->renderer, point1.x, point1.y, point2.x,
							  point2.y) == 0;
}

bool GameEngine::Graphics::Renderer::DrawRectangle(const Rect& rect) const {
	assert(renderer);

	SDL_Rect r = rect.GetRect();
	return SDL_RenderDrawRect(this->renderer, (rect.IsEmpty()) ? NULL : &r) ==
		   0;
}

bool GameEngine::Graphics::Renderer::DrawCircle(const Point& center,
												int radius) const {
	assert(renderer);
	assert(radius > 0);

	int points = 200;

	for (int i = 0; i < points; i++) {
		double angle = static_cast<double>(i) / points * 2.0f * M_PI;
		int x = static_cast<int>(center.x + radius * std::cos(angle));
		int y = static_cast<int>(center.y + radius * std::sin(angle));
		if (SDL_RenderDrawPoint(renderer, x, y) != 0)
			return false;
	}
	return true;
}

void GameEngine::Graphics::Renderer::Destroy() {
	if (renderer) {
		SDL_DestroyRenderer(this->renderer);
		this->renderer = nullptr;
	}
}
