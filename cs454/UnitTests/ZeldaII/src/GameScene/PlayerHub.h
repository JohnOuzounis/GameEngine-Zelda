#pragma once
#include <GameEngine/Graphics/UIElement.h>
#include <GameEngine/Resources.h>
#include <GameEngine/System.h>

class PlayerHub : public GameEngine::Graphics::UIElement {
   private:
	GameEngine::Graphics::Image* bg = nullptr;
	GameEngine::Graphics::Image* lifebg = nullptr;
	GameEngine::Graphics::Image* energybg = nullptr;
	GameEngine::Graphics::Image* numbers[10];

	GameEngine::Graphics::Rect energy = {39, 16, 9, 8};
	GameEngine::Graphics::Rect life = {111, 16, 9, 8};
	GameEngine::Graphics::Rect num = {200, 15, 8, 8};
	GameEngine::Graphics::Rect attack = {17, 15, 8, 8};

	int healthBars = 0, magicBars = 0, score = 0, dmg = 0;
	int width = 0, height = 0;

   public:
	PlayerHub(int width, int height) :width(width), height(height) { 
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		Image* menu = new Image(Resources::Get().GetAsset("bitmaps/menu.png"));
		bg = Image::Create(256, 24, {0, 0, 0, 255});
		lifebg = Image::Create(life.width, life.height, {0, 0, 0, 255});
		energybg = Image::Create(energy.width, energy.height, {0, 0, 0, 255});
		for (int i = 0; i < 10; i++) {
			numbers[i] = Image::Create(8, 8, {0, 0, 0, 255});
		}

		menu->BlitScaled({237, 2095, 256, 24}, *bg,
						 {0, 0, bg->GetWidth(), bg->GetHeight()});
		menu->BlitScaled({237, 2095, 9, 8}, *bg,
						 {energy.x, energy.y, 4 * energy.width, energy.height});
		menu->BlitScaled({237, 2095, 9, 8}, *bg,
						 {life.x, life.y, 4 * life.width, life.height});
		menu->BlitScaled(
			{237, 2095, 8, 8}, *bg,
			{num.x - 4 * num.width, num.y, 4 * num.width, num.height});
		menu->BlitScaled({237, 2095, 8, 8}, *bg,
						 {attack.x, attack.y, attack.width, attack.height});

		menu->BlitScaled({348, 2111, 9, 8}, *lifebg,
						 {0, 0, life.width, life.height});

		menu->BlitScaled({315, 2131, 9, 8}, *energybg,
						 {0, 0, energy.width, energy.height});

		for (int i = 0; i < 10; i++) {
			menu->BlitScaled({285 + 16 * i, 2074, num.width, num.height},
							 *numbers[i], {0, 0, num.width, num.height});
		}
		numbers[5]->BlitScaled(
			{0, 0, 8, 8}, *bg,
			{num.x + 2 * num.width, num.y, num.width, num.height});
		numbers[0]->BlitScaled(
			{0, 0, 8, 8}, *bg,
			{num.x + 3 * num.width, num.y, num.width, num.height});
		numbers[0]->BlitScaled(
			{0, 0, 8, 8}, *bg,
			{num.x + 4 * num.width, num.y, num.width, num.height});
		menu->BlitScaled({237, 2095, 8, 8}, *bg,
						 {num.x + 5 * num.width, num.y, num.width, num.height});

		System::Destroy(menu);
	}
	
	~PlayerHub() {
		if (bg)
			GameEngine::System::Destroy(bg);
		if (lifebg)
			GameEngine::System::Destroy(lifebg);
		if (energybg)
			GameEngine::System::Destroy(energybg);
	}

	void SetHealthBars(int bars) { healthBars = bars; }
	void SetMagicBars(int bars) { magicBars = bars; }
	void SetScore(int score) { this->score = score; }
	void SetDamage(int d) { dmg = d; }

	virtual void Draw(GameEngine::Graphics::Image& target) const override {
		GameEngine::Graphics::Image* copy = bg->Copy();

		for (int i = 0; i < healthBars; i++) {
			lifebg->Blit(
				{0, 0, 0, 0}, *copy,
				{life.x + i * life.width, life.y, life.width, life.height});
		}

		for (int i = 0; i < magicBars; i++) {
			energybg->Blit({0, 0, 0, 0}, *copy,
						   {energy.x + i * energy.width, energy.y, energy.width,
							energy.height});
		}

		std::string scoreString = std::to_string(score);

		for (int i = scoreString.length() - 1; i >= 0; i--) {
			int digit = scoreString[i] - '0';
			numbers[digit]->Blit(
				{0, 0, 0, 0}, *copy,
				{num.x - ((int)scoreString.length() - 1 - i) * num.width, num.y,
				 num.width, num.height});
		}

		std::string dmgString = std::to_string(dmg);

		for (int i = 0; i < dmgString.length(); i++) {
			int digit = dmgString[i] - '0';
			numbers[digit]->Blit({0, 0, 0, 0}, *copy,
								 {attack.x + i * attack.width, attack.y,
								  attack.width, attack.height});
		}

		copy->BlitScaled({0, 0, 0, 0}, target, {0, 0, width, height});
		delete copy;
	}
};