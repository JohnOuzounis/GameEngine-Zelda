#pragma once

#include <GameEngine/Debug.h>
#include <GameEngine/Scene.h>
#include <GameEngine/Audio.h>
#include <GameEngine/System.h>
#include <GameEngine/SceneManager.h>
#include <GameEngine/LatelyDestroyable.h>

#include <GameEngine/Animation/AnimatorManager.h>
#include <GameEngine/Animation/AnimationFilmHolder.h>

#include <GameEngine/Graphics/Button.h>
#include <GameEngine/Graphics/Panel.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Graphics/Renderer.h>

#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <GameEngine/JSON/StringProperty.h>

#include "MainMenu.h"
#include "../AudioManager.h"

class MenuScene : public GameEngine::Scene {
   private:
	GameEngine::Graphics::Window* window;
	GameEngine::Graphics::Renderer* renderer;

	MainMenu* menu = nullptr;
	GameEngine::Graphics::Button* button = nullptr;
	GameEngine::Graphics::Panel* ui = nullptr;

	GameEngine::Json::Configurator appConfig;

	void LoadFilms();

   public:
	MenuScene(GameEngine::Graphics::Window* window,
			  GameEngine::Graphics::Renderer* renderer,
			  GameEngine::app::Game& game)
		: Scene(game), window(window), renderer(renderer) {}

	virtual void Load() override {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		LoadFilms();
		Audio* audio = AudioManager::Get().GetAudio("audio/specialz.mp3");
		audio->Play();

		button = new Button();
		button->AddImage(Image::Create(250, 100, {100, 0, 255,50}));
		button->AddImage(Image::Create(250, 100, {100, 0, 255, 100}));
		button->AddImage(Image::Create(250, 100, {100, 0, 255, 150}));
		button->SetText(new Text("START", Text::Font::Roboto_Bold, 60));
		button->SetPosition({417, 260, 250, 100});

		button->SetOnClick(
			[]() { SceneManager::GetSceneManager().LoadNextScene(); });
		button->SetOnDraw([&](Image& target) {
			if (button) {
				Image* image = button->GetText()->GetImage();
				image->Blit(
					{0, 0, 0, 0}, button->GetImage(),
					{button->GetImage().GetWidth() / 2 - image->GetWidth() / 2,
					 button->GetImage().GetHeight() / 2 -
						 image->GetHeight() / 2,
					 button->GetImage().GetWidth(),
					 button->GetImage().GetHeight()});
				delete image;

				button->GetImage().BlitScaled(
					{0, 0, 0, 0}, target,
					{target.GetWidth() / 2 - button->GetImage().GetWidth() / 2,
					 target.GetHeight() / 2 - button->GetImage().GetHeight(),
					 button->GetImage().GetWidth(),
					 button->GetImage().GetHeight()});
			}
		});

		menu = new MainMenu(window->GetWidth(), window->GetHeight());
		ui = new Panel(*window);

		ui->Add(menu);
		ui->Add(button);
	}

	virtual void Save() override {}
	virtual void CleanUp() override {
		if (menu)
			GameEngine::System::Destroy(menu);

		if (ui)
			GameEngine::System::Destroy(ui);

		if (button)
			GameEngine::System::Destroy(button);

		GameEngine::DestructionManager::Get().Commit();
		GameEngine::AnimationFilmHolder::Get().CleanUp();
		GameEngine::ImageLoader::GetImageLoader().CleanUp();
		AudioManager::Get().CleanUp();
	}

	virtual void Render() override { 
		renderer->Clear();
		ui->Render(*renderer);
		renderer->Render();
	}

	virtual void Input() override { button->HandleEvent(); }

	virtual void ProgressAnimations() override {
		GameEngine::AnimatorManager::GetSingleton().Progress(
			GameEngine::Time::getTime());
	}

	virtual void CommitDestructions() override{
		GameEngine::DestructionManager::Get().Commit();
	}

};