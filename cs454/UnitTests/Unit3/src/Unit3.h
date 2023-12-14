#pragma once
#include <GameEngine/App.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Graphics/Renderer.h>
#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/Graphics/CircuralBackground.h>

class Unit3 : public GameEngine::app::App {
   public:
	GameEngine::Graphics::Window* window = nullptr;
	GameEngine::Graphics::Renderer* renderer = nullptr;

	GameEngine::Graphics::Tilemap* actionTilemap = nullptr;
	GameEngine::Graphics::Tilemap* bgTilemap = nullptr;
	GameEngine::Graphics::Image* displayBuffer = nullptr;
	GameEngine::Graphics::CircularBackground* bg = nullptr;

	virtual void Initialise() override;
	virtual void Load() override;
	virtual void Clear() override;
};