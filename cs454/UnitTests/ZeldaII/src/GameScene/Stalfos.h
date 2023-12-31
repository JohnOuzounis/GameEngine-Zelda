#pragma once
#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/MovingAnimator.h>

#include <GameEngine/SpriteManager.h>
#include <GameEngine/CollisionChecker.h>
#include <GameEngine/System.h>
#include <GameEngine/Debug.h>

#include "Enemy.h"

class Stalfos : public Enemy {
   private:
	bool spawned = false;
	bool extented = false;
	static int attackId;

	GameEngine::Graphics::Rect aoe;
	GameEngine::Graphics::Rect collider;

	void Move(int dx, int dy) {
		if (!CanMove(dx, dy, false))
			return;
		MoveCharacter(dx, dy);
		collider.x = position.x;
		collider.y = position.y + 18;
		((GameEngine::BoxCollider2D*)this->boundingArea)->SetRect(collider);
	}

   public:
	GameEngine::FrameListAnimation* left = nullptr;
	GameEngine::FrameListAnimation* right = nullptr;
	GameEngine::FrameListAnimator* animator = nullptr;

	GameEngine::MovingAnimation* fall = nullptr;
	GameEngine::MovingAnimator* fallAnimator = nullptr;

	Stalfos(int x,
			int y,
			int dx,
			int dy,
			float xvelocity,
			float yvelocity,
			GameEngine::AnimationFilm* film)
		: Enemy(x, y, dx, dy, xvelocity, yvelocity, film, "enemy", "stalfos") {

		Stalfos::attackId++;

		this->SetVisibility(false);
		aoe = {0, 0, 0, 0};
		collider = {0, 0, 16, 12};

		isLookingLeft = (film->GetId() == "stalfos.left");

		boundingArea = new GameEngine::BoxCollider2D(
			collider.x + 7, collider.y + 18, 16, 12, "stalfos");

		left = new GameEngine::FrameListAnimation("stalfos.left", {0, 1,2,3,4}, 0, -dx,
												  0, dx / xvelocity);

		right = new GameEngine::FrameListAnimation("stalfos.right", {0, 1,2,3,4}, 0, dx,
												   0, dx / xvelocity);

		animator = new GameEngine::FrameListAnimator();
		animator->SetOnAction([&](GameEngine::Animator* anim,
								  const GameEngine::Animation& a) {
			if (!canMove)
				return;
			if (!fallAnimator->HasFinished())
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
					if (prev != isLookingLeft)
						anim->Stop();
				}
			}

			if (((GameEngine::FrameListAnimator*)anim)->GetCurrFrame() != 4) {
				DespawnDamageSprite();
				this->Move(((GameEngine::FrameListAnimation*)&a)->GetDx(),
						   ((GameEngine::FrameListAnimation*)&a)->GetDy());
			}
			else {
				SpawnDamageSprite();
			}

			gravity.Check(position);

			this->SetFilm((GameEngine::AnimationFilm*)
							  GameEngine::AnimationFilmHolder::Get()
								  .GetFilm(a.GetId()));
			this->SetFrame(
				((GameEngine::FrameListAnimator*)anim)->GetCurrFrame());
		});

		fall = new GameEngine::MovingAnimation("stalfos.fall", 0, 0, dy,
											   dy / yvelocity);
		fallAnimator = new GameEngine::MovingAnimator();
		fallAnimator->SetOnStart([&](GameEngine::Animator* anim) {
			this->SetFilm((GameEngine::AnimationFilm*)
							  GameEngine::AnimationFilmHolder::Get()
								  .GetFilm("stalfos.fall"));
			this->SetFrame(1);
		});
		fallAnimator->SetOnFinish([&](GameEngine::Animator* anim) {
			this->SetFilm((GameEngine::AnimationFilm*)
							  GameEngine::AnimationFilmHolder::Get()
								  .GetFilm("stalfos.fall"));
			this->SetFrame(0);
			if (!extented) {
				aoe.width *= aoe.width;
				extented = true;
			}
		});
		fallAnimator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				if (!canMove)
					return;

				this->SetFilm((GameEngine::AnimationFilm*)
								  GameEngine::AnimationFilmHolder::Get()
									  .GetFilm("stalfos.fall"));
				this->SetFrame(1);

				this->Move(0, ((GameEngine::FrameListAnimation*)&a)->GetDy());

				gravity.Check(position);
				if (controller->GetGridmap().IsOnSolidGround(position)) {
					StopFalling();
				}
			});

		dmg = new GameEngine::FlashAnimation("stalfos.damage", 4, 0.1, 0.2);
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

	~Stalfos() {
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

	void SetAreaOfEffect(int width, int height) { aoe = {0, 0, width, height};	}

	void SpawnDamageSprite() const {
		int cw = 14;
		int ch = 8;
		int cx =
			(isLookingLeft) ? position.x - cw : position.x + position.width;
		int cy = y;

		GameEngine::Graphics::Sprite* dmgCol = new GameEngine::Graphics::Sprite(
			cx, cy, nullptr, std::to_string(attackId));
		dmgCol->SetCollider(new GameEngine::BoxCollider2D(
			cx, cy, cw, ch, std::to_string(attackId)));

		auto it = GameEngine::SpriteManager::GetSingleton()
					  .GetTypeList("player")
					  .begin();
		GameEngine::Graphics::Sprite* target =
			(it != GameEngine::SpriteManager::GetSingleton()
					   .GetTypeList("player")
					   .end())
				? *it
				: nullptr;

		if (!target)
			return;

		GameEngine::CollisionChecker::GetSingleton().Register(
			dmgCol, target,
			[&](GameEngine::Graphics::Sprite* s1,
					  GameEngine::Graphics::Sprite* s2) {
				if (onAttack)
					(onAttack)(s1, s2);
			});
	}

	void DespawnDamageSprite() {
		for (auto attack = GameEngine::SpriteManager::GetSingleton()
							   .GetTypeList(std::to_string(attackId))
							   .begin();
			 attack != GameEngine::SpriteManager::GetSingleton()
						   .GetTypeList(std::to_string(attackId))
						   .end(); attack++) {
			if ((*attack)->IsAlive()) {
				delete (*attack)->GetCollider();
				(*attack)->Destroy();
			}
		}
	}

	virtual void Start() override {

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
				if (animator && animator->HasFinished()) {
					if (!spawned)
						this->SetVisibility(true);
					spawned = true;
					animator->Start((isLookingLeft) ? left : right,
									GameEngine::Time::getTime());
				}
			}
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