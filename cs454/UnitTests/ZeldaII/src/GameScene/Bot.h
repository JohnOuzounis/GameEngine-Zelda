#pragma once
#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/MovingAnimator.h>

#include <GameEngine/SpriteManager.h>
#include <GameEngine/System.h>
#include <GameEngine/Debug.h>

#include "Enemy.h"

class Bot : public Enemy {
   private:
	GameEngine::Graphics::Rect aoe;

   public:
	GameEngine::FrameListAnimation* idle = nullptr;
	GameEngine::FrameListAnimator* animator = nullptr;

	GameEngine::MovingAnimation* fall = nullptr;
	GameEngine::MovingAnimator* fallAnimator = nullptr;
	GameEngine::MovingAnimation* jump = nullptr;
	GameEngine::MovingAnimator* jumpAnimator = nullptr;

	Bot(int x,
		int y,
		int dx,
		int dy,
		float xvelocity,
		float yvelocity,
		GameEngine::AnimationFilm* film)
		: Enemy(x, y, dx, dy, xvelocity, yvelocity, film, "enemy", "bot") {
		isLookingLeft = GameEngine::Math::Random(0, 1) == 0;

		aoe = {0, 0, 0, 0};
		boundingArea = new GameEngine::BoxCollider2D(x + 7, y, 16, 16, "bot");

		idle = new GameEngine::FrameListAnimation("bot.idle", {0, 1, 2}, 1, 0,
												  0, 0.07);

		animator = new GameEngine::FrameListAnimator();
		animator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				gravity.Check(position);

				this->SetFilm((GameEngine::AnimationFilm*)
								  GameEngine::AnimationFilmHolder::Get()
									  .GetFilm(a.GetId()));
				this->SetFrame(
					((GameEngine::FrameListAnimator*)anim)->GetCurrFrame());
			});

		jump = new GameEngine::MovingAnimation("bot.jump", 4, dx, -dy,
											   dy / yvelocity);
		jumpAnimator = new GameEngine::MovingAnimator();
		jumpAnimator->SetOnStart([&](GameEngine::Animator* anim) {
			if (animator)
				animator->Stop();

			this->SetFrame(1);
		});
		jumpAnimator->SetOnAction([&](GameEngine::Animator* anim,
									  const GameEngine::Animation& a) {
			if (!canMove)
				return;

			auto it = GameEngine::SpriteManager::GetSingleton()
						  .GetTypeList("player")
						  .begin();
			GameEngine::Graphics::Sprite* target =
				(it != GameEngine::SpriteManager::GetSingleton()
						   .GetTypeList("player")
						   .end())
					? *it
					: nullptr;

			if (!aoe.IsEmpty() && target && target->IsAlive())
			{
				aoe.x = position.x - aoe.width/2;
				aoe.x = (aoe.x < 0) ? 0 : aoe.x;
				aoe.y = position.y - aoe.height/2;
				aoe.y = (aoe.y < 0) ? 0 : aoe.y;
				
				if (aoe.In(target->GetBox().x, target->GetBox().y)) {
					isLookingLeft = (target->GetBox().x - position.x < 0);
				}
			}

			int dx = ((GameEngine::FrameListAnimation*)&a)->GetDx();
			dx = (isLookingLeft) ? -dx : dx;
			if (!CanMove(dx, 0, false)) {
				isLookingLeft = !isLookingLeft;
			}

			auto prev = position;
			MoveCharacter(dx, ((GameEngine::FrameListAnimation*)&a)->GetDy());
			gravity.Check(position);
			if (prev.y == position.y) {
				jumpAnimator->Stop();
			}
		});
		jumpAnimator->SetOnFinish([&](GameEngine::Animator* anim) {
			if (anim->IsAlive())
				Fall();
		});

		fall = new GameEngine::MovingAnimation("bot.fall", 0, dx, dy,
											   dy / yvelocity);
		fallAnimator = new GameEngine::MovingAnimator();
		fallAnimator->SetOnAction([&](GameEngine::Animator* anim,
									  const GameEngine::Animation& a) {
			if (!canMove)
				return;

			int dx = ((GameEngine::FrameListAnimation*)&a)->GetDx();
			dx = (isLookingLeft) ? -dx : dx;
			if (!CanMove(dx, ((GameEngine::FrameListAnimation*)&a)->GetDy(), false)) {
				isLookingLeft = !isLookingLeft;
			}

			MoveCharacter(dx, ((GameEngine::FrameListAnimation*)&a)->GetDy());
			gravity.Check(position);
			if (controller->GetGridmap().IsOnSolidGround(position)) {
				StopFalling();
			}
		});
		fallAnimator->SetOnFinish([&](GameEngine::Animator* anim) {
			if (anim->IsAlive() && animator && animator->HasFinished())
				animator->Start(idle, GameEngine::Time::getTime());
		});

		dmg = new GameEngine::FlashAnimation("bot.damage", 4, 0.1, 0.2);
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

	~Bot() {
		GameEngine::System::Destroy(idle);
		GameEngine::System::Destroy(jump);
		GameEngine::System::Destroy(fall);
		GameEngine::System::Destroy(dmg);
		GameEngine::System::Destroy(controller);

		damageAnimator->Destroy();
		damageAnimator = nullptr;
		animator->Destroy();
		animator = nullptr;
		fallAnimator->Destroy();
		fallAnimator = nullptr;
		jumpAnimator->Destroy();
		jumpAnimator = nullptr;
	}

	void SetAreaOfEffect(int width, int height) {
		this->aoe = {0, 0, width, height};
	}

	virtual void Start() override {
		if (!jumpAnimator || !fallAnimator || !jump)
			return;

		if (jumpAnimator->HasFinished() && fallAnimator->HasFinished() &&
			animator->HasFinished())
			jumpAnimator->Start(jump, GameEngine::Time::getTime());
	}

	void Fall() {
		if (!jumpAnimator || !fallAnimator || !fall)
			return;

		if (fallAnimator->HasFinished() && jumpAnimator->HasFinished())
			fallAnimator->Start(fall, GameEngine::Time::getTime());
	}
	void StopFalling() {
		if (fallAnimator)
			fallAnimator->Stop();
	}
};