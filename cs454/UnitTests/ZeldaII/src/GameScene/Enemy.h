#pragma once
#include <GameEngine/Animation/FlashAnimator.h>

#include <GameEngine/Graphics/Sprite.h>
#include <GameEngine/Graphics/Gridmap.h>

#include <GameEngine/CharacterController.h>
#include <GameEngine/BoxCollider2D.h>
#include <GameEngine/Time.h>

#include <functional>
#include "Health.h"

class Enemy : public GameEngine::Graphics::Sprite {
   public:
	using OnDeath = std::function<void(void)>;
	using OnAttack = std::function<void(GameEngine::Graphics::Sprite*,
										GameEngine::Graphics::Sprite*)>;
	using OnTakeDamage = std::function<void(void)>;

   protected:
	OnDeath onDeath;
	OnAttack onAttack;
	OnTakeDamage onTakeDamage;

	GameEngine::CharacterController* controller = nullptr;
	int offsetX = 0;
	int offsetY = 0;

	float xvelocity = 0;
	float yvelocity = 0;
	int dx = 0;
	int dy = 0;

	Health health;
	int damage = 0;

	bool isLookingLeft = false;
	bool canMove = true;
	GameEngine::Graphics::Rect position;

	GameEngine::FlashAnimation* dmg = nullptr;
	GameEngine::FlashAnimator* damageAnimator = nullptr;


	void Die() {
		canMove = false;
		this->SetVisibility(false);
		if (onDeath)
			(onDeath)();

		this->Destroy();
	}

   public:
	int Attack() const { return damage; }

	void TakeDamage(int damage) {
		if (!damageAnimator || !damageAnimator->HasFinished())
			return;

		damageAnimator->Start(dmg, GameEngine::Time::getTime());
		health.TakeDamage(damage);

		if (onTakeDamage)
			(onTakeDamage)();

		if (health.IsEmpty()) {
			damageAnimator->Stop();
			Die();
		}
	}

	bool IsLookingLeft() const { return isLookingLeft; }
	bool IsMoveable() const { return canMove; }

	int GetDamage() const { return damage; }
	void SetDamage(int dmg) { damage = dmg; }

	const Health& GetHealth() const { return health; }
	void SetHealth(int healthBars, int healthPerBar) {
		health.SetHealth(healthBars, healthPerBar);
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

	bool CanMove(int dx, int dy, bool checkEdges) {
		auto original = position;
		controller->Move(dx, dy);
		bool isBlocked = position.Equals(original);
		position = original;

		auto row = (position.y + position.height + 1) /
						controller->GetGridmap().GetGridTileHeight();
		auto colright = (position.x + position.width + 1) /
						controller->GetGridmap().GetGridTileWidth();
		auto colleft =
			(position.x - 1) / controller->GetGridmap().GetGridTileWidth();
		colleft = (colleft < 0) ? 0 : colleft;

		bool hasReachedEdge = controller->GetGridmap().GetGridTile(
								  row, colright) == GRID_EMPTY_TILE ||
							  controller->GetGridmap().GetGridTile(
								  row, colleft) == GRID_EMPTY_TILE;

		return !isBlocked && (!checkEdges || !hasReachedEdge);
	}

   public:
	std::string name;
	Enemy(int x,
		  int y,
		  int dx,
		  int dy,
		  float xvelocity,
		  float yvelocity,
		  GameEngine::AnimationFilm* film,
		  const std::string& _typeId = "",
		  const std::string& name = "enemy")
		: GameEngine::Graphics::Sprite(x, y, film, _typeId),
		  name(name),
		  dx(dx),
		  dy(dy),
		  xvelocity(xvelocity),
		  yvelocity(yvelocity) {
		this->SetVisibility(true);
		this->SetZorder(5);
	}

	void SetOnDeath(OnDeath d) { onDeath = d; }
	void SetOnAttack(OnAttack a) { onAttack = a; }
	void SetOnTakeDamage(OnTakeDamage dmg) { onTakeDamage = dmg; }

	virtual void Start() = 0;
};