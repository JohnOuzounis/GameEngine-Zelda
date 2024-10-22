#pragma once
#include <GameEngine/Graphics/UIElement.h>
#include <GameEngine/Graphics/Text.h>
#include <GameEngine/System.h>

class PauseMenu : public GameEngine::Graphics::UIElement {
   private:
	GameEngine::Graphics::Image* bg = nullptr;
	GameEngine::Graphics::Text* text = nullptr;

   public:
	bool enabled = false;

	virtual void Draw(GameEngine::Graphics::Image& target) const override {
		using namespace GameEngine::Graphics;

		if (enabled) {
			Image* image = text->GetImage();

			image->Blit({0, 0, 0, 0}, *bg,
						{bg->GetWidth() / 2 - image->GetWidth() / 2,
						 bg->GetHeight() / 2 - image->GetHeight() / 2,
						 image->GetWidth(), image->GetHeight()});
			delete image;

			bg->Blit({0, 0, 0, 0}, target,
					 {0, 0, bg->GetWidth(), bg->GetHeight()});
		}
	}

	PauseMenu(int width, int height) {
		using namespace GameEngine::Graphics;

		bg = Image::Create(width, height, {123, 123, 123, 255});
		text = new Text("PAUSED", Text::Font::Roboto_Bold, 40);
	}
	~PauseMenu() {
		if (bg)
			GameEngine::System::Destroy(bg);
		if (text)
			GameEngine::System::Destroy(text);
	}
};