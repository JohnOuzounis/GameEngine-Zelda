#pragma once
#include <GameEngine/Graphics/Sprite.h>
#include <GameEngine/BoxCollider2D.h>
#include <GameEngine/System.h>
#include <GameEngine/Input.h>
#include <functional>

class Item : public GameEngine::Graphics::Sprite {
   public:
	using ItemEffect = std::function<void(Item*)>;

   private:
	bool used = false;
	ItemEffect effect;

   public:
	std::string name;

	Item(int x, int y, GameEngine::AnimationFilm* film, const std::string& name)
		: GameEngine::Graphics::Sprite(x, y, film, "item"), name(name) {
		this->SetVisibility(true);

		boundingArea = new GameEngine::BoxCollider2D(
			x, y, film->GetFrameBox(frameNo).width,
			film->GetFrameBox(frameNo).height, name);
	}
	~Item() { GameEngine::System::Destroy(boundingArea); }

	void SetEffect(ItemEffect e) { effect = e; }

	void UseItem() {
		if (!used && effect) {
			(effect)(this);
			used = true;
		}
	}
};