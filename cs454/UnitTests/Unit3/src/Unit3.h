#pragma once
#include <GameEngine/App.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Graphics/Renderer.h>
#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/Graphics/CircuralBackground.h>
#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Graphics/Sprite.h>
#include <GameEngine/Animation/FrameListAnimation.h>
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/MovingAnimator.h>
#include <GameEngine/Animation/FlashAnimator.h>

class Unit3 : public GameEngine::app::App {
   public:
	GameEngine::Graphics::Window* window = nullptr;
	GameEngine::Graphics::Renderer* renderer = nullptr;

	GameEngine::Graphics::Tilemap* actionTilemap = nullptr;
	GameEngine::Graphics::Tilemap* bgTilemap = nullptr;
	GameEngine::Graphics::Image* displayBuffer = nullptr;
	GameEngine::Graphics::CircularBackground* bg = nullptr;

	GameEngine::Graphics::Sprite* link;
	GameEngine::FrameListAnimation* a;
	GameEngine::FrameListAnimation* b;
	GameEngine::MovingAnimation* c;
	GameEngine::FrameListAnimator* anim;
	GameEngine::MovingAnimator* m;
	GameEngine::FlashAnimation* fl;
	GameEngine::FlashAnimator* fla;

	virtual void Initialise() override;
	virtual void Load() override;
	virtual void Clear() override;
};