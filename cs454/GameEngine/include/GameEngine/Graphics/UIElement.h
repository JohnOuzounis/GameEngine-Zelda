#pragma once
#include <GameEngine/Graphics/Image.h>

namespace GameEngine {

namespace Graphics {

class UIElement {
   public:
	virtual void Draw(Image& target) const = 0;
};

}  // namespace Graphics
}  // namespace GameEngine