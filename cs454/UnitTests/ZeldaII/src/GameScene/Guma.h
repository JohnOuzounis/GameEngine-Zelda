#pragma once
#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/MovingAnimator.h>

#include <GameEngine/CollisionChecker.h>
#include <GameEngine/Debug.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/System.h>

#include "Enemy.h"

class Guma : public Enemy {
   private:
	static int attackId;
	int id = 0;
	bool isLookingAtTarget = false;
	GameEngine::Graphics::Rect aoe;

   public:
	GameEngine::FrameListAnimation* left = nullptr;
	GameEngine::FrameListAnimation* right = nullptr;
	GameEngine::FrameListAnimator* animator = nullptr;

	GameEngine::MovingAnimation* fall = nullptr;
	GameEngine::MovingAnimator* fallAnimator = nullptr;

	GameEngine::FrameListAnimator* dmgAnim = nullptr;

	Guma(int x,
		 int y,
		 int dx,
		 int dy,
		 float xvelocity,
		 float yvelocity,
		 GameEngine::AnimationFilm* film)
		: Enemy(x, y, dx, dy, xvelocity, yvelocity, film, "enemy", "guma") {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		id = Guma::attackId++;
		aoe = {0, 0, 0, 0};

		isLookingLeft = (film->GetId() == "guma.left");
		boundingArea = new GameEngine::BoxCollider2D(x + 7, y, 16, 32, "guma");

		left = new GameEngine::FrameListAnimation("guma.left", {3, 2, 1, 0}, 0,
												  -dx, 0, dx / xvelocity);

		right = new GameEngine::FrameListAnimation("guma.right", {3, 2, 1, 0},
												   0, dx, 0, dx / xvelocity);

		animator = new GameEngine::FrameListAnimator();
		animator->SetOnAction([&](GameEngine::Animator* anim,
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

			if (!aoe.IsEmpty() && target && target->IsAlive()) {
				aoe.x = position.x - aoe.width / 2;
				aoe.x = (aoe.x < 0) ? 0 : aoe.x;
				aoe.y = position.y - aoe.height / 2;
				aoe.y = (aoe.y < 0) ? 0 : aoe.y;

				if (aoe.In(target->GetBox().x, target->GetBox().y)) {
					auto prev = isLookingLeft;
					isLookingLeft = (target->GetBox().x - position.x < 0);
					isLookingAtTarget = true;
					if (prev != isLookingLeft)
						anim->Stop();
				} else {
					isLookingAtTarget = false;
				}
			}

			if (((GameEngine::FrameListAnimator*)anim)->GetCurrFrame() == 1) {
				DespawnDamageSprite();
				SpawnDamageSprite();
			}
			if (CanMove(((GameEngine::FrameListAnimation*)&a)->GetDx(),
						((GameEngine::FrameListAnimation*)&a)->GetDy(), true))
				MoveCharacter(((GameEngine::FrameListAnimation*)&a)->GetDx(),
							  ((GameEngine::FrameListAnimation*)&a)->GetDy());
			else if (!isLookingAtTarget) {
				isLookingLeft = !isLookingLeft;
				MoveCharacter(-((GameEngine::FrameListAnimation*)&a)->GetDx(),
							  ((GameEngine::FrameListAnimation*)&a)->GetDy());
				anim->Stop();
			}

			gravity.Check(position);

			this->SetFilm((GameEngine::AnimationFilm*)
							  GameEngine::AnimationFilmHolder::Get()
								  .GetFilm(a.GetId()));
			this->SetFrame(
				((GameEngine::FrameListAnimator*)anim)->GetCurrFrame());
		});

		fall = new GameEngine::MovingAnimation("guma.fall", 0, 0, dy,
											   dy / yvelocity);
		fallAnimator = new GameEngine::MovingAnimator();
		fallAnimator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				if (!canMove)
					return;

				MoveCharacter(0, ((GameEngine::FrameListAnimation*)&a)->GetDy());

				gravity.Check(position);
				if (controller->GetGridmap().IsOnSolidGround(position)) {
					StopFalling();
				}
			});

		dmg = new GameEngine::FlashAnimation("guma.damage", 4, 0.1, 0.2);
		damageAnimator = new GameEngine::FlashAnimator();
		damageAnimator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				this->SetVisibility(!this->IsVisible());
			});

		dmgAnim = new FrameListAnimator();
		dmgAnim->SetOnAction([&](Animator* anim, const Animation& a) {
			auto attack = GameEngine::SpriteManager::GetSingleton()
							  .GetTypeList("guma" + std::to_string(id))
							  .begin();
			if (attack == GameEngine::SpriteManager::GetSingleton()
							  .GetTypeList("guma" + std::to_string(id))
							  .end()) {
				anim->Stop();
				return;
			}
			(*attack)->SetFrame(((FrameListAnimator*)anim)->GetCurrFrame());

			Rect r = ((BoxCollider2D*)(*attack)->GetCollider())->GetRect();
			r.Move(((FrameListAnimation&)a).GetDx(), 0);
			((BoxCollider2D*)(*attack)->GetCollider())->SetRect(r);

			(*attack)->SetHasDirectMotion(true).Move(
				((FrameListAnimation&)a).GetDx(), 0);
		});

		gravity.SetGravity(true);
		gravity.SetOnSolidGround([&](const GameEngine::Graphics::Rect& rect) {
			return controller->GetGridmap().IsOnSolidGround(rect);
		});
		gravity.SetOnStartFalling([&]() { Fall(); });
		gravity.SetOnStopFalling([&]() { StopFalling(); });
	}

	~Guma() {
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
		dmgAnim->Destroy();
		dmgAnim = nullptr;
	}

	void SetAreaOfEffect(int width, int height) { aoe = {0, 0, width, height}; }

	void SpawnDamageSprite() const {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		int dmgDx = (isLookingLeft) ? -8 : 8;
		int cw = 8;
		int ch = 16;
		int cx =
			(isLookingLeft) ? position.x - cw : position.x + position.width;
		int cy = y;

		Sprite* club = new Sprite(
			cx, cy, (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("guma.club"),
					   "guma" + std::to_string(id));
		club->SetVisibility(true);
		club->SetCollider(new BoxCollider2D(cx, cy, cw, ch,
											"guma" + std::to_string(id)));

		dmgAnim->Start(
			new FrameListAnimation("guma.club" + std::to_string(id),
								   {0, 1, 2}, 15, dmgDx, 0, 0.09),
			Time::getTime());

		auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
		Sprite* target =
			(it != SpriteManager::GetSingleton().GetTypeList("player").end())
				? *it
				: nullptr;

		if (!target)
			return;

		CollisionChecker::GetSingleton().Register(club, target,
												  [&](Sprite* s1, Sprite* s2) {
													  if (onAttack)
														  (onAttack)(s1, s2);
												  });
	}

	void DespawnDamageSprite() {
		for (auto attack = GameEngine::SpriteManager::GetSingleton()
							   .GetTypeList("guma" + std::to_string(id))
							   .begin();
			 attack != GameEngine::SpriteManager::GetSingleton()
						   .GetTypeList("guma" + std::to_string(id))
						   .end();
			 attack++) {
			if ((*attack)->IsAlive()) {
				delete (*attack)->GetCollider();
				(*attack)->Destroy();
				dmgAnim->Stop();
			}
		}
	}

	virtual void Start() override {
		if (animator && animator->HasFinished()) {
			animator->Start((isLookingLeft) ? left : right,
							GameEngine::Time::getTime());
		}
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