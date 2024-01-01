#pragma once
#include <GameEngine/Graphics/Text.h>
#include <GameEngine/Graphics/UIElement.h>

#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Animation/FrameRangeAnimator.h>

#include <GameEngine/Debug.h>
#include <GameEngine/Resources.h>
#include <GameEngine/System.h>
#include <GameEngine/Time.h>
#include <vector>

class MainMenu : public GameEngine::Graphics::UIElement {
   private:
	GameEngine::Graphics::Image* bg = nullptr;
	std::vector<GameEngine::Graphics::Text*> texts;

	GameEngine::FrameRangeAnimator* animator = nullptr;
	GameEngine::FrameRangeAnimation* animation = nullptr;

	GameEngine::Graphics::Rect frameBox;
	GameEngine::AnimationFilm* film = nullptr;
	int frameNo = 0;

	void SetFrame(int i) {
		assert(film);
		assert(i < film->GetTotalFrames());
		frameBox = film->GetFrameBox(frameNo = i);
	}

   public:
	MainMenu(int width, int height) {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		film = (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("bg.idle");
		SetFrame(0);

		bg = Image::Create(width, height, {0, 0, 0, 255});

		int fontSize = 20;
		Text::Font font = Text::Font::Roboto_BoldItalic;

		texts.push_back(new Text("JOHN OUZOUNIS, csd4705", font, fontSize));
		texts.push_back(new Text("University of Crete", font, fontSize));
		texts.push_back(
			new Text("Department of Computer Science", font, fontSize));
		texts.push_back(
			new Text("CS -454. Development of Intelligent Interfaces and Games",
					 font, fontSize));
		texts.push_back(
			new Text("Term Project, Fall Semester 2023", font, fontSize));

		animation = new FrameRangeAnimation("bg.idle", 0, 7, 0, 0, 0, 0.3);
		animator = new FrameRangeAnimator();
		animator->SetOnAction([&](Animator* anim, const Animation& a) {
			if (anim->IsAlive())
				SetFrame(((FrameRangeAnimator*)anim)->GetCurrFrame());
		});
		animator->Start(animation, Time::getTime());
	}

	~MainMenu() {
		if (bg)
			GameEngine::System::Destroy(bg);

		for (int i = 0; i < texts.size(); i++) {
			if (texts[i])
				GameEngine::System::Destroy(texts[i]);
		}
		texts.clear();

		if (animator) {
			animator->Destroy();
			animator = nullptr;
		}

		if (animation)
			GameEngine::System::Destroy(animation);
	}

	virtual void Draw(GameEngine::Graphics::Image& target) const override {
		using namespace GameEngine::Graphics;

		film->GetBitmap()->BlitScaled(frameBox, *bg,
									  {0, 0, bg->GetWidth(), bg->GetHeight()});

		for (int i = 0; i < texts.size(); i++) {
			Image* image = texts[i]->GetImage();
			image->Blit({0, 0, 0, 0}, *bg,
						{bg->GetWidth() / 2 - image->GetWidth() / 2,
						 bg->GetHeight() - ((int)texts.size() -i+1) * image->GetHeight(), image->GetWidth(),
						 image->GetHeight()});
			delete image;
		}

		bg->Blit({0, 0, 0, 0}, target,
				 {0, 0, target.GetWidth(), target.GetHeight()});
	}
};