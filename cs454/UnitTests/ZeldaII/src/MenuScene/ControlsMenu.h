#pragma once
#include <GameEngine/Graphics/Text.h>
#include <GameEngine/Graphics/UIElement.h>

#include <GameEngine/Debug.h>
#include <GameEngine/Resources.h>
#include <GameEngine/System.h>
#include <vector>

class ControlsMenu : public GameEngine::Graphics::UIElement {
   private:
	GameEngine::Graphics::Image* bg = nullptr;
	std::vector<GameEngine::Graphics::Text*> texts;

   public:
	bool enabled = false;

	ControlsMenu(int width, int height) {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		bg = Image::Create(width, height, {0, 0, 0, 255});

		Text::Font font = Text::Font::OpenSans_BoldItalic;
		int fontSize = 30;

		texts.push_back(new Text("MOVE LEFT: LEFT ARROW", font, fontSize));
		texts.push_back(new Text("MOVE RIGHT: RIGHT ARROW", font, fontSize));
		texts.push_back(new Text("JUMP: A", font, fontSize));
		texts.push_back(new Text("CROUCH: DOWN ARROW", font, fontSize));
		texts.push_back(new Text("ACTIVATE SUPERJUMP SPELL: F", font, fontSize));
		texts.push_back(new Text("ATTACK: B", font, fontSize));
		texts.push_back(new Text("CROUCH ATTACK: DOWN + B", font, fontSize));
		texts.push_back(new Text("PAUSE/RESUME: ESCAPE", font, fontSize));
		texts.push_back(new Text("ACTIVATE SHIELD SPELL: G", font, fontSize));
	}

	~ControlsMenu() {
		if (bg)
			GameEngine::System::Destroy(bg);

		for (int i = 0; i < texts.size(); i++) {
			if (texts[i])
				GameEngine::System::Destroy(texts[i]);
		}
		texts.clear();
	}

	virtual void Draw(GameEngine::Graphics::Image& target) const override {
		using namespace GameEngine::Graphics;

		if (enabled) {
			for (int i = 0; i < texts.size(); i++) {
				Image* image = texts[i]->GetImage();
				int x = (i <= texts.size() / 2)
							? 0
							: bg->GetWidth()/2;
				int y = (i <= texts.size() / 2)
							? i * image->GetHeight()
							: (i - (int)texts.size() / 2-1) * image->GetHeight();
				image->Blit({0, 0, 0, 0}, *bg,
							{x, y, image->GetWidth(), image->GetHeight()});
				delete image;
			}

			bg->Blit({0, 0, bg->GetWidth(), bg->GetHeight()}, target,
					 {0, 0, bg->GetWidth(), bg->GetHeight()});
		}
	}
};