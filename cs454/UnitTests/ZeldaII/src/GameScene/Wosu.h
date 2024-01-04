#pragma once
#include "Enemy.h"
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/MovingAnimator.h>
#include <GameEngine/Animation/AnimationFilmHolder.h>

#include <GameEngine/Debug.h>
#include <GameEngine/System.h>

class Wosu : public Enemy {
   public:
	GameEngine::FrameListAnimation* left = nullptr;
	GameEngine::FrameListAnimation* right = nullptr;
	GameEngine::FrameListAnimator* animator = nullptr;

	GameEngine::MovingAnimation* fall = nullptr;
	GameEngine::MovingAnimator* fallAnimator = nullptr;

	Wosu(int x,
		 int y,
		 int dx,
		 int dy,
		 float xvelocity,
		 float yvelocity,
		 GameEngine::AnimationFilm* film)
		: Enemy(x, y, dx, dy, xvelocity, yvelocity, film, "enemy", "wosu") {
		isLookingLeft = (film->GetId() == "wosu.left");

		boundingArea = new GameEngine::BoxCollider2D(x + 7, y, 16, 32, "wosu");

		left = new GameEngine::FrameListAnimation("wosu.left", {0, 1}, 0, -dx,
												  0, dx / xvelocity);

		right = new GameEngine::FrameListAnimation("wosu.right", {0, 1}, 0, dx,
												   0, dx / xvelocity);

		animator = new GameEngine::FrameListAnimator();
		animator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				if (!canMove)
					return;

				if (CanMove(((GameEngine::FrameListAnimation*)&a)->GetDx(),
							((GameEngine::FrameListAnimation*)&a)->GetDy(), true)) {
					MoveCharacter(
						((GameEngine::FrameListAnimation*)&a)->GetDx(),
						((GameEngine::FrameListAnimation*)&a)->GetDy());
				} else if (fallAnimator->HasFinished()) {
					if (CanMove(-((GameEngine::FrameListAnimation*)&a)->GetDx(),
								((GameEngine::FrameListAnimation*)&a)->GetDy(),
								true)) {
						isLookingLeft = !isLookingLeft;
						MoveCharacter(
							-((GameEngine::FrameListAnimation*)&a)->GetDx(),
							((GameEngine::FrameListAnimation*)&a)->GetDy());
						anim->Stop();
					}
				}

				gravity.Check(position);

				this->SetFilm((GameEngine::AnimationFilm*)
								  GameEngine::AnimationFilmHolder::Get()
									  .GetFilm(a.GetId()));
				this->SetFrame(
					((GameEngine::FrameListAnimator*)anim)->GetCurrFrame());
			});

		fall = new GameEngine::MovingAnimation("wosu.fall", 0, 0, dy,
											   dy / yvelocity);
		fallAnimator = new GameEngine::MovingAnimator();
		fallAnimator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				if (!canMove)
					return;

				MoveCharacter(((GameEngine::FrameListAnimation*)&a)->GetDx(),
							  ((GameEngine::FrameListAnimation*)&a)->GetDy());
				gravity.Check(position);
				if (controller->GetGridmap().IsOnSolidGround(position)) {
					StopFalling();
				}
			});

		dmg = new GameEngine::FlashAnimation("wosu.damage", 4, 0.1, 0.2);
		damageAnimator = new GameEngine::FlashAnimator();
		damageAnimator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				this->SetVisibility(!this->IsVisible());
			});

		gravity.SetGravity(true);
		gravity.SetOnSolidGround([&](const GameEngine::Graphics::Rect& rect) {
			return controller->GetGridmap().IsOnSolidGround(rect);
		});
		gravity.SetOnStartFalling([&]() { Fall(); });
		gravity.SetOnStopFalling([&]() { StopFalling(); });
	}

	~Wosu() {
		GameEngine::System::Destroy(left);
		GameEngine::System::Destroy(right);
		GameEngine::System::Destroy(fall);
		GameEngine::System::Destroy(dmg);
		GameEngine::System::Destroy(controller);

		damageAnimator->Destroy();
		damageAnimator = nullptr;
		animator->Destroy();
		animator = nullptr;
		fallAnimator->Destroy();
		fallAnimator = nullptr;
	}
		
	virtual void Start() override {
		if (animator && animator->HasFinished())
			animator->Start((isLookingLeft) ? left : right,
							GameEngine::Time::getTime());
	}

	void Fall() {
		if (!fallAnimator || !fall)
			return;

		if (fallAnimator->HasFinished())
			fallAnimator->Start(fall, GameEngine::Time::getTime());
	}

	void StopFalling() {
		if (fallAnimator)
			fallAnimator->Stop();
	}
};