#define LINK_IDLE_LEFT "link.idle.left"
#define LINK_IDLE_RIGHT "link.idle.right"
#define LINK_WALK_LEFT "link.walk.left"
#define LINK_WALK_RIGHT "link.walk.right"
#define LINK_ATTACK_LEFT "link.attack.left"
#define LINK_ATTACK_RIGHT "link.attack.right"
#define LINK_CROUCH_LEFT "link.crouch.left"
#define LINK_CROUCH_RIGHT "link.crouch.right"
#define LINK_CROUCH_ATTACK_LEFT "link.crouch.attack.left"
#define LINK_CROUCH_ATTACK_RIGHT "link.crouch.attack.right"

#include <GameEngine/Animation/AnimationFilm.h>
#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/MovingAnimator.h>
#include <GameEngine/Animation/FlashAnimator.h>
#include <GameEngine/Animation/TickAnimator.h>

#include <GameEngine/SceneManager.h>
#include <GameEngine/CharacterController.h>
#include <GameEngine/Debug.h>
#include <GameEngine/Graphics/Gridmap.h>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Graphics/Sprite.h>
#include <GameEngine/Input.h>
#include <GameEngine/System.h>
#include <GameEngine/Time.h>
#include <GameEngine/BoxCollider2D.h>
#include <GameEngine/SpriteManager.h>
#include <GameEngine/CollisionChecker.h>

#include <cassert>
#include <unordered_map>

#include "Health.h"
#include "Enemy.h"
#include "Item.h"
#include "../AudioManager.h"

class Player : public GameEngine::Graphics::Sprite {
	GameEngine::CharacterController* controller = nullptr;
	int offsetX = 0;
	int offsetY = 0;

	using Animations =
		std::unordered_map<std::string, GameEngine::FrameListAnimation*>;
	Animations animations;
	GameEngine::FrameListAnimator* animator = nullptr;

	GameEngine::MovingAnimation* fall = nullptr;
	GameEngine::MovingAnimation* jump = nullptr;
	GameEngine::MovingAnimator* jumpAnimator = nullptr;
	GameEngine::MovingAnimator* fallAnimator = nullptr;

	GameEngine::FlashAnimation* dmg = nullptr;
	GameEngine::FlashAnimator* damageAnimator = nullptr;

	float xvelocity = 0;
	float yvelocity = 0;
	int dx = 0;
	int dy = 0;
	int damage = 0;

   public:
	Health health;
	int scorePoints = 0;
	int magicPoints = 0;

	bool isCrouched = false;
	bool isLookingLeft = false;
	bool canMove = true;
	GameEngine::Graphics::Rect position;
	std::vector<std::string> collectedItems;

	Player(int _x,
		   int _y,
		   float xvelocity,
		   float yvelocity,
		   int dx,
		   int dy,
		   GameEngine::AnimationFilm* film)
		: GameEngine::Graphics::Sprite(_x, _y, film, "player"),
		  xvelocity(xvelocity),
		  yvelocity(yvelocity),
		  dx(dx),
		  dy(dy) {
		position = GameEngine::Graphics::Rect(x, y, 0, 0);
		this->SetVisibility(true);
		this->SetZorder(10);

		boundingArea = new GameEngine::BoxCollider2D(x, y, 0, 0, "player");

		animations[LINK_WALK_LEFT] = new GameEngine::FrameListAnimation(
			LINK_WALK_LEFT, {3, 2, 1, 0}, 0, -dx, 0, dx / xvelocity);

		animations[LINK_WALK_RIGHT] = new GameEngine::FrameListAnimation(
			LINK_WALK_RIGHT, {0, 1, 2, 3}, 0, dx, 0, dx / xvelocity);

		animations[LINK_IDLE_LEFT] =
			new GameEngine::FrameListAnimation(LINK_IDLE_LEFT, {0}, 0, 0, 0, 1);

		animations[LINK_IDLE_RIGHT] = new GameEngine::FrameListAnimation(
			LINK_IDLE_RIGHT, {0}, 0, 0, 0, 1);

		animations[LINK_CROUCH_LEFT] = new GameEngine::FrameListAnimation(
			LINK_CROUCH_LEFT, {0}, 0, 0, 0, 1);

		animations[LINK_CROUCH_RIGHT] = new GameEngine::FrameListAnimation(
			LINK_CROUCH_RIGHT, {0}, 0, 0, 0, 1);

		animations[LINK_ATTACK_LEFT] = new GameEngine::FrameListAnimation(
			LINK_ATTACK_LEFT, {0, 1, 2}, 1, 0, 0, 0.3);

		animations[LINK_ATTACK_RIGHT] = new GameEngine::FrameListAnimation(
			LINK_ATTACK_RIGHT, {0, 1, 2}, 1, 0, 0, 0.3);

		animations[LINK_CROUCH_ATTACK_LEFT] =
			new GameEngine::FrameListAnimation(LINK_CROUCH_ATTACK_LEFT, {0, 1},
											   1, 0, 0, 0.3);

		animations[LINK_CROUCH_ATTACK_RIGHT] =
			new GameEngine::FrameListAnimation(LINK_CROUCH_ATTACK_RIGHT, {0, 1},
											   1, 0, 0, 0.3);

		animator = new GameEngine::FrameListAnimator();
		animator->SetOnAction([&](GameEngine::Animator* anim,
								  const GameEngine::Animation& a) {
			if (!canMove)
				return;

			if (((a.GetId() == LINK_ATTACK_LEFT ||
				  a.GetId() == LINK_ATTACK_RIGHT) &&
				 ((GameEngine::FrameListAnimator*)anim)->GetCurrFrame() == 1) ||
				((a.GetId() == LINK_CROUCH_ATTACK_LEFT ||
				  a.GetId() == LINK_CROUCH_ATTACK_RIGHT) &&
				 ((GameEngine::FrameListAnimator*)anim)->GetCurrFrame() == 0))
				SpawnDamageSprite();
			else
				DespawnDamageSprite();

			if (a.GetId() == LINK_WALK_LEFT || a.GetId() == LINK_WALK_RIGHT)
				MoveCharacter(((GameEngine::FrameListAnimation*)&a)->GetDx(),
							  ((GameEngine::FrameListAnimation*)&a)->GetDy());
			gravity.Check(position);

			SetFilm((GameEngine::AnimationFilm*)
						GameEngine::AnimationFilmHolder::Get()
							.GetFilm(a.GetId()));
			SetFrame(((GameEngine::FrameListAnimator*)anim)->GetCurrFrame());
		});

		jump = new GameEngine::MovingAnimation("link.jump", 6, 0, -dy,
											   dy / yvelocity);
		jumpAnimator = new GameEngine::MovingAnimator();
		jumpAnimator->SetOnAction(
			[&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
				if (!canMove)
					return;

				auto prev = position;
				MoveCharacter(((GameEngine::FrameListAnimation*)&a)->GetDx(),
							  ((GameEngine::FrameListAnimation*)&a)->GetDy());
				gravity.Check(position);
				if (prev.y == position.y) {
					jumpAnimator->Stop();
				}
			});
		jumpAnimator->SetOnFinish([&](GameEngine::Animator* anim) {
			if (anim->IsAlive())
				Fall();
		});

		fall = new GameEngine::MovingAnimation("link.fall", 0, 0, dy,
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

		dmg = new GameEngine::FlashAnimation("link.damage", 4, 0.1, 0.2);
		damageAnimator = new GameEngine::FlashAnimator();
		damageAnimator->SetOnStart([](GameEngine::Animator* anim) {
			AudioManager::Get().PlayEffect("audio/hit.wav");
			});
		damageAnimator->SetOnAction([&](GameEngine::Animator* anim, const GameEngine::Animation& a) {
			this->SetVisibility(!this->IsVisible());
		});

		gravity.SetGravity(true);
		gravity.SetOnSolidGround([&](const GameEngine::Graphics::Rect& rect) {
			return controller->GetGridmap().IsOnSolidGround(rect);
		});
		gravity.SetOnStartFalling([&]() { Fall(); });
		gravity.SetOnStopFalling([&]() { StopFalling(); });
	}

	~Player() {
		for (auto& anim : animations) {
			GameEngine::System::Destroy(anim.second);
		}
		animations.clear();
		collectedItems.clear();

		GameEngine::System::Destroy(controller);
		GameEngine::System::Destroy(fall);
		GameEngine::System::Destroy(jump);
		GameEngine::System::Destroy(dmg);
		GameEngine::System::Destroy(boundingArea);

		animator->Destroy();
		animator = nullptr;

		fallAnimator->Destroy();
		fallAnimator = nullptr;

		jumpAnimator->Destroy();
		jumpAnimator = nullptr;
		
		damageAnimator->Destroy();
		damageAnimator = nullptr;
	}

	void SetDamage(int dmg) { damage = dmg; }
	int GetDamage() { return damage; }
	void SetHealth(int healthBars, int healthPerBar) {
		health.SetHealth(healthBars, healthPerBar);
	}

	void SpawnDamageSprite() {
		int cw = (isCrouched) ? 12 : 16;
		int ch = 8;
		int cx = (isLookingLeft) ? position.x - cw : position.x + position.width;
		int cy = (isCrouched) ? y + position.height - ch : y;
		int cdmg = (isCrouched) ? damage / 2 : damage;

		GameEngine::Graphics::Sprite* dmgCol =
			new GameEngine::Graphics::Sprite(cx, cy, nullptr, "link.attacker");
		dmgCol->SetCollider(
			new GameEngine::BoxCollider2D(cx, cy, cw, ch, "link.attacker"));
		for (auto enemy = GameEngine::SpriteManager::GetSingleton()
							  .GetTypeList("enemy")
							  .begin();
			 enemy != GameEngine::SpriteManager::GetSingleton()
						  .GetTypeList("enemy")
						  .end();
			 ++enemy) {
			GameEngine::CollisionChecker::GetSingleton().Register(
				dmgCol, *enemy,
				[&,cdmg](GameEngine::Graphics::Sprite* s1,
					   GameEngine::Graphics::Sprite* s2) {
					if (s2->IsAlive()) {
						if (this->health.IsFull())
						{
							GameEngine::Debug::Log("spawned");
							GameEngine::System::WaitForSeconds(0.5, []() {
								GameEngine::Debug::Log("despawned");
							});
							// spawn explosion at s2.collider
							// play explosion sound
							// destroy after few seconds
						}
						((Enemy*)s2)->TakeDamage(cdmg);
						if (s1->IsAlive()) {
							delete s1->GetCollider();
							s1->Destroy();
						}
					}
				});
		}

		for (auto item = GameEngine::SpriteManager::GetSingleton()
							  .GetTypeList("item")
							  .begin();
			 item != GameEngine::SpriteManager::GetSingleton()
						  .GetTypeList("item")
						  .end();
			 ++item) {
			GameEngine::CollisionChecker::GetSingleton().Register(
				dmgCol, *item, [&, cdmg](Sprite * s1, Sprite * s2) {
					Item* item = (Item*)s2;
					CollectItem(item->name);
					item->UseItem();
					item->Destroy();
					GameEngine::CollisionChecker::GetSingleton().Cancel(s1, s2);
				});
		}
	}
	void DespawnDamageSprite() {
		auto lit = GameEngine::SpriteManager::GetSingleton()
					   .GetTypeList("link.attacker")
					   .begin();
		if (lit != GameEngine::SpriteManager::GetSingleton()
			.GetTypeList("link.attacker")
			.end())
		{
			if ((*lit)->IsAlive()) {
				delete (*lit)->GetCollider();
				(*lit)->Destroy();
			}
		}
	}

	bool CheckBlockedAttack(const GameEngine::Graphics::Rect& other) const {
		if (!isCrouched)
			return false;
		int cw = 8;
		int ch = 16;
		int cx =
			(isLookingLeft) ? position.x - cw : position.x + position.width;
		int cy = y + position.height - ch;

		GameEngine::BoxCollider2D me(cx, cy, cw, ch, "me");
		GameEngine::BoxCollider2D box(other.x, other.y, other.width, other.height, "other");

		return me.Overlap(box);
	}
	void TakeDamage(int damage) {
		if (!damageAnimator->HasFinished())
			return;

		damageAnimator->Start(dmg, GameEngine::Time::getTime());
		health.TakeDamage(damage);

		if (health.IsEmpty()) {
			damageAnimator->Stop();
			Die();
		}
	}
	
	void Die() {
		canMove = false;
		this->SetVisibility(false);
		AudioManager::Get().Play("audio/gameover.wav", 128);
		this->Destroy();

		GameEngine::TickAnimator* reset = new GameEngine::TickAnimator();
		reset->SetOnFinish([](GameEngine::Animator* anim) {
			delete ((GameEngine::TickAnimator*)anim)->GetAnimation();
			anim->Destroy();

			GameEngine::SceneManager::GetSceneManager().LoadScene(
				GameEngine::SceneManager::GetSceneManager()
					.GetCurrentSceneIndex());
			});
		reset->Start(new GameEngine::TickAnimation("game.reset", 2, 1, true),
					 GameEngine::Time::getTime());
	}

	void CollectItem(const std::string& item) {
		collectedItems.push_back(item);
	}

	bool HasItem(const std::string& name) {
		auto it = std::find(collectedItems.begin(), collectedItems.end(), name);
		return it != collectedItems.end();
	}

	void Update() {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		if (Input::GetKeyDown(Event::Right)) {
			isLookingLeft = false;
			Walk();
		}
		if (Input::GetKeyUp(Event::Right)) {
			Idle();
		}

		if (Input::GetKeyDown(Event::Left)) {
			isLookingLeft = true;
			Walk();
		}
		if (Input::GetKeyUp(Event::Left))
			Idle();

		if (Input::GetKeyDown(Event::A)) {
			Jump();
		}

		if (Input::GetKeyDown(Event::B)) {
			Attack();
		}

		if (Input::GetKeyDown(Event::Down)) {
			Crouch();
		}
		if (Input::GetKeyUp(Event::Down)) {
			Stand();
		}
	}

	void SetCharacterPosition(int offsetX, int offsetY, int width, int height) {
		position =
			GameEngine::Graphics::Rect(x + offsetX, y + offsetY, width, height);
		((GameEngine::BoxCollider2D*)this->boundingArea)->SetRect(position);

		this->offsetX = offsetX;
		this->offsetY = offsetY;
	}

	void SetCharacterController(const GameEngine::Graphics::Gridmap& gridmap) {
		assert(!controller);
		controller = new GameEngine::CharacterController(position, gridmap);
	}

	void MoveCharacter(int dx, int dy) {
		controller->Move(dx, dy);
		((GameEngine::BoxCollider2D*)this->boundingArea)->SetRect(position);

		x = position.x - offsetX;
		y = position.y - offsetY;
	}

	bool CanMove(int dx, int dy) {
		auto original = position;
		controller->Move(dx, dy);
		bool res = !position.Equals(original);
		position = original;

		return res;
	}

	void Crouch() {
		using namespace GameEngine;
		auto anim = (isLookingLeft) ? animations[LINK_CROUCH_LEFT]
									: animations[LINK_CROUCH_RIGHT];
		isCrouched = true;

		if (anim) {
			animator->Stop();
			animator->Start(anim, Time::getTime());
		}
	}

	void Stand() {
		using namespace GameEngine;
		auto anim = (isLookingLeft) ? animations[LINK_IDLE_LEFT]
									: animations[LINK_IDLE_RIGHT];
		isCrouched = false;

		if (!CanMove(0, -1)) {
			isCrouched = true;
			anim = (FrameListAnimation*)animator->GetAnimation();
		}

		if (anim) {
			animator->Stop();
			animator->Start(anim, Time::getTime());
		}
	}

	void Walk() {
		if (!animator)
			return;

		if (animator->GetAnimation() != nullptr &&
			animator->GetAnimation()->GetId() != LINK_WALK_LEFT &&
			animator->GetAnimation()->GetId() != LINK_WALK_RIGHT)
			animator->Stop();

		if (animator->HasFinished()) {
			auto anim = (isLookingLeft) ? animations[LINK_WALK_LEFT]
										: animations[LINK_WALK_RIGHT];
			if (anim)
				animator->Start(anim, GameEngine::Time::getTime());
		}
	}

	void Idle() {
		if (!animator)
			return;

		animator->Stop();
		auto anim = (isLookingLeft) ? animations[LINK_IDLE_LEFT]
									: animations[LINK_IDLE_RIGHT];
		if (anim)
			animator->Start(anim, GameEngine::Time::getTime());
	}

	void Attack() {
		if (!animator)
			return;

		if (animator->GetAnimation()->GetId() != LINK_ATTACK_LEFT &&
			animator->GetAnimation()->GetId() != LINK_ATTACK_RIGHT &&
			animator->GetAnimation()->GetId() != LINK_CROUCH_ATTACK_LEFT &&
			animator->GetAnimation()->GetId() != LINK_CROUCH_ATTACK_RIGHT)
			animator->Stop();

		if (animator->HasFinished()) {
			auto anim = (isLookingLeft)
							? (isCrouched) ? animations[LINK_CROUCH_ATTACK_LEFT]
										   : animations[LINK_ATTACK_LEFT]
						: (isCrouched) ? animations[LINK_CROUCH_ATTACK_RIGHT]
									   : animations[LINK_ATTACK_RIGHT];
			if (anim)
				animator->Start(anim, GameEngine::Time::getTime());
		}
	}

	void Jump() {
		if (!jumpAnimator || !fallAnimator || !jump)
			return;

		if (jumpAnimator->HasFinished() && fallAnimator->HasFinished())
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