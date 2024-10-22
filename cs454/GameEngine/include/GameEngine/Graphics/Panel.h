#pragma once
#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Renderer.h>
#include <GameEngine/Graphics/Texture.h>
#include <GameEngine/Graphics/UIElement.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/System.h>
#include <vector>

namespace GameEngine {
namespace Graphics {

class Panel {
   private:
	const Window& window;
	Image* panel;
	std::vector<UIElement*> uis;

   public:
	Panel(const Window& window)
		: window(window) {
		panel = Image::Create(window.GetWidth(), window.GetHeight(),
							  {123, 123, 123, 255});
		panel->SetColorKey({123, 123, 123, 255}, true);
	}
	~Panel() {
		System::Destroy(panel);
		uis.clear();
	}

	void Add(UIElement* ui) { uis.push_back(ui); }

	void Render(const Renderer& renderer) {
		panel->Clear({123,123,123,255});
		for (int i = 0; i < uis.size(); i++) {
			uis[i]->Draw(*panel);
		}

		Texture* texture = new Texture(renderer.GetRenderer(), *panel);
		renderer.Copy(*texture, {0, 0, 0, 0},
					  {0, 0, window.GetWidth(), window.GetHeight()});
		System::Destroy(texture);
	}
};

}  // namespace Graphics
}  // namespace GameEngine