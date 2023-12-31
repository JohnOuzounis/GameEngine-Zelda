#pragma once
#include <GameEngine/Animation/MovingAnimator.h>
#include <GameEngine/Debug.h>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Graphics/Sprite.h>
#include <GameEngine/BoxCollider2D.h>
#include <GameEngine/System.h>
#include <GameEngine/Time.h>
#include <GameEngine/Input.h>
#include <functional>

class Elevator {
   public:
	using GridUpdater =
		std::function<void(const GameEngine::Graphics::Rect&, unsigned char)>;
	using OnMove = std::function<void(int, int)>;
	using OnStartMoving = std::function<void(void)>;
	using OnStopMoving = std::function<void(void)>;

	GridUpdater updater;
	OnMove onMove;
	OnStartMoving onStart;
	OnStopMoving onStop;

	GameEngine::Graphics::Sprite *top, *bot;
	GameEngine::Graphics::Rect topPosition, botPosition;
	int offsetX = 0, offsetY = 0;

	bool isLowered = false;
	bool called = false;
	int dx = 0, dy = 0;

	GameEngine::MovingAnimation* anim = nullptr;
	GameEngine::MovingAnimator* animator = nullptr;

	Elevator(GameEngine::Graphics::Sprite* top,
			 GameEngine::Graphics::Sprite* bot,
			 bool isLowered,
			 int dx,
			 int dy)
		: top(top), bot(bot), isLowered(isLowered), dx(dx), dy(dy) {
		auto r = top->GetFilm()->GetCollider(top->GetFrame());
		offsetX = r.x;
		offsetY = r.y;


		topPosition = GameEngine::Graphics::Rect(
			top->GetBox().x + r.x, top->GetBox().y + r.y, r.width, r.height);

		botPosition = GameEngine::Graphics::Rect(
			bot->GetBox().x + r.x, bot->GetBox().y + r.y, r.width, r.height);
		bot->SetCollider(new GameEngine::BoxCollider2D(
			botPosition.x, botPosition.y, botPosition.width, botPosition.height,
			bot->GetTypeId()));

		animator = new GameEngine::MovingAnimator();
		animator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				if (this->isLowered) {
					Move(-this->dx, -this->dy);
				} else {
					Move(this->dx, this->dy);
				}
			});
		animator->SetOnStart([&](GameEngine::Animator* anim) {
			if (onStart)
				(onStart)();

			Update(0);
		});
		animator->SetOnFinish([&](GameEngine::Animator* anim) {
			if (!anim->IsAlive())
				return;

			this->isLowered = !this->isLowered;

			Update(15);

			if (onStop)
				(onStop)();

			if (this->anim) {
				GameEngine::System::Destroy(this->anim);
			}
		});
	}

	~Elevator() {
		if (top) {
			top->Destroy();
			top = nullptr;
		}

		if (bot) {
			bot->Destroy();
			bot = nullptr;
		}

		if (anim) {
			GameEngine::System::Destroy(anim);
		}

		if (animator) {
			animator->Destroy();
			animator = nullptr;
		}
	}

	void SetUpdater(const GridUpdater& g) { updater = g; }
	void SetOnMove(const OnMove& m) { onMove = m; }
	void SetOnStartMoving(const OnStartMoving& start) { onStart = start; }
	void SetOnStopMoving(const OnStopMoving& stop) { onStop = stop; }

	void Display(GameEngine::Graphics::Image* dest,
				 const GameEngine::Graphics::Rect& dpyArea,
				 const GameEngine::Clipper& clipper) {
		if (top && top->IsVisible())
			top->Display(dest, dpyArea, clipper);

		if (bot && bot->IsVisible())
			bot->Display(dest, dpyArea, clipper);
	}

	void Move(int dx, int dy) {
		//if (updater) {
		//	(updater)(topPosition, 0);
		//	(updater)(botPosition, 0);
		//}

		topPosition.Move(dx, dy);
		botPosition.Move(dx, dy);
		((GameEngine::BoxCollider2D*)bot->GetCollider())->SetRect(botPosition);

		top->SetHasDirectMotion(true).Move(dx, dy);
		bot->SetHasDirectMotion(true).Move(dx, dy);

		//if (updater) {
		//	(updater)(topPosition, 15);
		//	(updater)(botPosition, 15);
		//}

		if (onMove)
			(onMove)(dx, dy);
	}

	void Move(int reps, double delay) {
		anim = new GameEngine::MovingAnimation("elevator.move", reps, dx, dy,
											   delay);
		if (animator)
			animator->Start(anim, GameEngine::Time::getTime());
	}

	void CallElevator() {
		if (GameEngine::Input::GetKeyDown(GameEngine::Graphics::Event::Down))
			called = true;
		if (GameEngine::Input::GetKeyUp(GameEngine::Graphics::Event::Down))
			called = false;
	}

	void Update(unsigned char val) {
		if (updater) {
			(updater)(topPosition, val);
			(updater)(botPosition, val);
		}
	}
};