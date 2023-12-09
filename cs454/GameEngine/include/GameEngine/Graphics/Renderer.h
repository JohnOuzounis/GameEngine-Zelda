#pragma once

#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Graphics/Color.h>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Graphics/Texture.h>

#include <SDL2/SDL.h>

namespace GameEngine {
namespace Graphics {

/// <summary>
/// DONT USE DRAWING FUNCTIONS TO RENDER ANYTHING
/// THE POSITION DOESNT MATCH THE POSITION OF THE BLITS
/// </summary>
class Renderer {
   private:
	SDL_Renderer* renderer;

   public:
	enum Flags { Accelerated = SDL_RENDERER_ACCELERATED };

   public:
	Renderer(Window& window, int index, Uint32 flags);

	~Renderer() { Destroy(); }

   public:
	SDL_Renderer* GetRenderer() const;

	bool Clear() const;
	bool Copy(Texture& texture, const Rect& src, const Rect& dest) const;
	void Destroy();

   public:
	void Render() const;
	bool SetDrawColor(const Color& color);
	bool SetTarget(Texture* texture);

	bool DrawPoint(const Point& point) const;
	bool DrawLine(const Point& point1, const Point& point2) const; 
	bool DrawRectangle(const Rect& rect) const;
	bool DrawCircle(const Point& center, int radius) const;
};

}  // namespace Graphics
}  // namespace GameEngine