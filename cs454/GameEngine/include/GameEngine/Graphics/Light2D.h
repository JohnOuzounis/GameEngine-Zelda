#pragma once

#include <GameEngine/Graphics/Color.h>
#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/System.h>
#include <string>
#include <vector>

namespace GameEngine {
namespace Graphics {

class Light2D {
   public:
	enum Type { Spot, Point, Ambient };

   private:
	Color color;
	Rect position;
	Type type;
	Image* image;
	std::vector<std::string> paths;

   public:
	Light2D(const Color color, const Rect& position, const Type& type);
	~Light2D() {
		System::Destroy(image);
		paths.clear();
	}

	Rect GetPosition() const { return position; }
	void SetPosition(Rect& position);

	Color GetColor() const { return color; }
	void SetColor(Color& color);

	Image* GetLight() const { return image; }
};

}  // namespace Graphics
}  // namespace GameEngine