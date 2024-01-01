#pragma once
#include <GameEngine/BoxCollider2D.h>
#include <GameEngine/Graphics/Sprite.h>
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Input.h>
#include <GameEngine/Time.h>
#include <GameEngine/System.h>
#include <functional>

#include "../AudioManager.h"

class Door : public GameEngine::Graphics::Sprite {
   public:
	using GridUpdater =
		std::function<void(const GameEngine::Graphics::Rect&, unsigned char)>;

   private:
	bool isOpening = false;
	int offsetX = 0, offsetY = 0;
	GridUpdater updater;
	GameEngine::FrameListAnimation* open = nullptr;
	GameEngine::FrameListAnimation* close = nullptr;
	GameEngine::FrameListAnimator* animator = nullptr;


   public:
	bool isUnlocked = false;

	Door(int x, int y, GameEngine::AnimationFilm* film, const std::string& tag)
		: GameEngine::Graphics::Sprite(x, y, film, tag) {
		this->SetVisibility(true);

		offsetX = film->GetCollider(frameNo).x;
		offsetY = film->GetCollider(frameNo).y;
		boundingArea = new GameEngine::BoxCollider2D(
			x - offsetX, y - offsetX, film->GetCollider(frameNo).width,
			film->GetCollider(frameNo).height, tag);

		open = new GameEngine::FrameListAnimation(
			"door.open",
			{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}, 1, 0, 0,
			0.03);

		close = new GameEngine::FrameListAnimation(
			"door.open",
			{16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0}, 1, 0, 0,
			0.03);

		animator = new GameEngine::FrameListAnimator();
		animator->SetOnStart([&](GameEngine::Animator* anim) {
			if (this->isOpening) {
				AudioManager::Get().PlayEffect("audio/door_open.wav");
				if (!AudioManager::Get()
						 .GetAudio("audio/battle_area.wav")
						 ->IsPlaying()) {
					AudioManager::Get().Play("audio/battle_area.wav", 128);
					AudioManager::Get().Stop("audio/boss_battle.wav");
				}
			}
			else {
				AudioManager::Get().Stop("audio/battle_area.wav");
				AudioManager::Get().PlayEffect("audio/Sound Effect (25).wav");
				AudioManager::Get().Play("audio/boss_battle.wav", 128);
				this->SetVisibility(true);
				this->Update(15);
			}

		});
		animator->SetOnAction([&](GameEngine::Animator* anim,
								  const GameEngine::Animation& a) {
			this->SetFilm((GameEngine::AnimationFilm*)
						GameEngine::AnimationFilmHolder::Get()
							.GetFilm(a.GetId()));
			this->SetFrame(((GameEngine::FrameListAnimator*)anim)->GetCurrFrame());
		});
		animator->SetOnFinish([&](GameEngine::Animator* anim) {
			if (isOpening) {
				this->SetVisibility(false);
				this->Update(0);
				this->Destroy();
			}
		});
	}

	~Door() {
		GameEngine::System::Destroy(boundingArea);
		GameEngine::System::Destroy(open);
		GameEngine::System::Destroy(close);
		animator->Destroy();
	}

	void SetUpdater(const GridUpdater& g) { updater = g; }
	void Update(unsigned char val) {
		if (updater) {
			(updater)(((GameEngine::BoxCollider2D*)boundingArea)->GetRect(),
					  val);
		}
	}

	void OpenDoor() {
		if (!isUnlocked)
			return;
		if (animator->HasFinished()) {
			isOpening = true;
			animator->Start(open, GameEngine::Time::getTime());
		}
	}

	void CloseDoor() {
		if (animator->HasFinished()) {
			isOpening = false;
			animator->Start(close, GameEngine::Time::getTime());
		}
	}

	void Unlock() {
		if (GameEngine::Input::GetKeyDown(GameEngine::Graphics::Event::Down))
			isUnlocked = true;
		if (GameEngine::Input::GetKeyUp(GameEngine::Graphics::Event::Down))
			isUnlocked = false;
	}
};