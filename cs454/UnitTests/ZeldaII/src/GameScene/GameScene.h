#include <cassert>
#include <vector>

#include <GameEngine/Clipper.h>
#include <GameEngine/Input.h>
#include <GameEngine/Resources.h>
#include <GameEngine/Scene.h>
#include <GameEngine/System.h>
#include <GameEngine/Camera.h>
#include <GameEngine/LatelyDestroyable.h>
#include <GameEngine/CollisionChecker.h>
#include <GameEngine/SpriteManager.h>

#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <GameEngine/Animation/AnimatorManager.h>

#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Renderer.h>
#include <GameEngine/Graphics/Texture.h>
#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/Graphics/Window.h>
#include <GameEngine/Graphics/Panel.h>

#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <GameEngine/JSON/StringProperty.h>

#include "../../../Unit1/src/TilemapMaker.h"
#include "Player.h"
#include "Elevator.h"
#include "Door.h"
#include "Item.h"
#include "Wosu.h"
#include "Stalfos.h"
#include "Bot.h"
#include "PauseMenu.h"
#include "../AudioManager.h"

#include <GameEngine/Debug.h>
class GameScene : public GameEngine::Scene {
   private:
	GameEngine::Graphics::Window* window;
	GameEngine::Graphics::Renderer* renderer;

	std::vector<GameEngine::Graphics::Rect> areas;
	GameEngine::Graphics::Tilemap* map = nullptr;
	GameEngine::Graphics::Image* displayBuffer = nullptr;

	GameEngine::Camera* camera = nullptr;
	bool isCameraBounded = true;
	int scrollSpeed = 0;
	bool canScroll = true;

	int lavaDmg = 0;
	int sfxVol = 70;

	Elevator* elevator = nullptr;
	Elevator* elevator2 = nullptr;
	Elevator* elevator3 = nullptr;

	PauseMenu* pauseMenu = nullptr;
	GameEngine::Graphics::Panel* ui = nullptr;

	GameEngine::Json::Configurator appConfig;

	void LoadMaps();
	void LoadFilms();
	void LoadAreaBounds();
	void LoadVars();
	void LoadAudio() const;

	void MakeDoll(int x, int y);
	void MakeGauntlet(int x, int y, int dmg);
	void MakePointbag(int x, int y, int points);
	void MakeHealPotion(int x, int y);
	void MakeHeart(int x, int y);
	void MakePotion(int x,
					int y,
					const std::string& film,
					const std::string& tag, int points);
	void MakeKey(int x,
					int y,
					const std::string& film,
					const std::string& tag);

	void MakeLava(int x, int y, int w, int dmg);

	template <typename T>
	T* MakeCharacter(int x,
					 int y,
					 const std::string& configName,
					 GameEngine::AnimationFilm* film);
	Wosu* MakeWosu(int x, int y, GameEngine::AnimationFilm* film);
	Stalfos* MakeStalfos(int x, int y, GameEngine::AnimationFilm* film);
	Bot* MakeBot(int x, int y);

	Player* MakePlayer();

	Door* MakeDoor();
	Door* MakeGate();

	Elevator* MakeElevator(int x,
						   int yTop,
						   int yBot,
						   int dx,
						   int dy,
						   int reps,
						   double delay,
						   std::string topTag,
						   std::string botTag);

	GameEngine::Camera* MakeCamera();
	Item* MakeItem(int x,
				  int y,
				  const std::string& film,
				  const std::string& tag);

	void SpawnEnemies();
	void SpawnItems();

	const GameEngine::Graphics::Rect& GetArea(
		const GameEngine::Graphics::Rect& position);

   public:
	GameScene(GameEngine::Graphics::Window* window,
			  GameEngine::Graphics::Renderer* renderer, GameEngine::app::Game& game)
		: Scene(game), window(window), renderer(renderer) {}

	virtual void Load() override {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;
		using namespace GameEngine::Json;

		displayBuffer = Image::Create(16 * 24, 16 * 16, {0, 0, 0, 255});

		pauseMenu = new PauseMenu(window->GetWidth(), window->GetHeight());
		ui = new GameEngine::Graphics::Panel(*window);
		ui->Add(pauseMenu);

		LoadMaps();
		LoadFilms();
		LoadAreaBounds();
		LoadVars();
		LoadAudio();

		MakePlayer();
		MakeDoor();
		MakeGate();
		camera = MakeCamera();


		SpawnEnemies();
		SpawnItems();

		MakeLava(2384, 496, 208, lavaDmg);
		MakeLava(3440, 1008, 384, lavaDmg);

		elevator = MakeElevator(1792, 176, 224, 0, 1, 32 * 8, 0.015,
								"elevator.top1", "elevator.bot1");
		elevator2 = MakeElevator(2384, 416, 464, 1, 0, 22 * 8, 0.015,
								 "elevator.top2", "elevator.bot2");
		elevator3 = MakeElevator(2768, 432, 480, 0, 1, 32 * 8, 0.015,
								 "elevator.top3", "elevator.bot3");

		AudioManager::Get().Play("audio/battle_area.wav", 128);

		this->AddPauseResume([&]() {
			if (this->IsPaused()) {
				auto theme =
					AudioManager::Get().GetAudio("audio/battle_area.wav");
				if (!theme->IsPlaying())
					theme =
						AudioManager::Get().GetAudio("audio/boss_battle.wav");
				if (theme->IsPlaying())
					theme->Pause();

				pauseMenu->enabled = true;
			} else {
				auto it =
					SpriteManager::GetSingleton().GetTypeList("player").begin();
				Player* player =
					(it !=
					 SpriteManager::GetSingleton().GetTypeList("player").end())
						? (Player*)*it
						: nullptr;
				if (player)
					player->Idle();

				auto theme =
					AudioManager::Get().GetAudio("audio/battle_area.wav");
				if (!theme->IsPlaying() && !theme->IsPaused())
					theme =
						AudioManager::Get().GetAudio("audio/boss_battle.wav");
				theme->Resume();

				AnimatorManager::GetSingleton().TimeShift(Time::getTime() -
														  game.GetPauseTime());

				pauseMenu->enabled = false;
			}
		});
	}

	virtual void Save() override {}
	virtual void CleanUp() override {
		if (map)
			GameEngine::System::Destroy(map);

		if (camera)
			GameEngine::System::Destroy(camera);

		if (elevator)
			GameEngine::System::Destroy(elevator);

		if (elevator2)
			GameEngine::System::Destroy(elevator2);

		if (elevator3)
			GameEngine::System::Destroy(elevator3);

		if (displayBuffer)
			GameEngine::System::Destroy(displayBuffer);

		if (pauseMenu)
			GameEngine::System::Destroy(pauseMenu);

		if (ui)
			GameEngine::System::Destroy(ui);

		GameEngine::CollisionChecker::GetSingleton().CleanUp();
		GameEngine::SpriteManager::GetSingleton().CleanUp();
		GameEngine::AnimationFilmHolder::Get().CleanUp();
		AudioManager::Get().CleanUp();
		CommitDestructions();
	}

	virtual void Render() override {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		displayBuffer->Clear({0, 0, 0, 255});

		auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
		Player* player =
			(it != SpriteManager::GetSingleton().GetTypeList("player").end())
				? (Player*)*it
				: nullptr;
		if (!player)
			camera->Follow(nullptr);

		auto viewWin = camera->GetPosition();
		if (isCameraBounded && player) {
			map->SetBounds(GetArea(player->position));
			viewWin = camera->GetPositionClamped(map->GetBounds());
		} else
			map->RemoveBounds();

		map->SetView(viewWin);
		map->Scroll(0, 0);

		map->Display(*displayBuffer, {0, 0, displayBuffer->GetWidth(),
									  displayBuffer->GetHeight()});

#ifdef DEBUG
		map->GetTilemap()->Clear({0, 0, 0, 255});
		Image* tile = Image::Create(map->GetGridmap()->GetGridTileWidth(),
									map->GetGridmap()->GetGridTileHeight(),
									{0, 0, 0, 255});
		for (int i = 0; i < map->GetGridmap()->GetGridRows(); i++) {
			for (int j = 0; j < map->GetGridmap()->GetGridColumns(); j++) {
				if (map->GetGridmap()->GetGridTile(i, j) == GRID_SOLID_TILE) {
					tile->Clear({255, 0, 0, 255});
				} else if (map->GetGridmap()->GetGridTile(i, j) ==
						   GRID_EMPTY_TILE)
					tile->Clear({0, 255, 0, 255});
				tile->Blit({0, 0, tile->GetWidth(), tile->GetHeight()},
						   *map->GetTilemap(),
						   {j * tile->GetWidth(), i * tile->GetHeight(),
							tile->GetWidth(), tile->GetHeight()});
			}
		}
		delete tile;

		if (player) {
			auto r = player->position;
			tile = Image::Create(r.width, r.height, {0, 0, 255, 255});
			tile->Blit({0, 0, r.width, r.height}, *map->GetTilemap(),
					   player->position);
			delete tile;
		}

		tile = Image::Create(32, 16, {0, 0, 255, 255});
		tile->BlitScaled({0, 0, 32, 16}, *map->GetTilemap(),
						 elevator->topPosition);
		tile->BlitScaled({0, 0, 32, 16}, *map->GetTilemap(),
						 elevator->botPosition);

		tile->BlitScaled({0, 0, 32, 16}, *map->GetTilemap(),
						 elevator3->topPosition);
		tile->BlitScaled({0, 0, 32, 16}, *map->GetTilemap(),
						 elevator3->botPosition);

		tile->BlitScaled({0, 0, 32, 16}, *map->GetTilemap(),
						 elevator2->topPosition);
		tile->BlitScaled({0, 0, 32, 16}, *map->GetTilemap(),
						 elevator2->botPosition);
		delete tile;

		for (auto eit =
				 SpriteManager::GetSingleton().GetTypeList("enemy").begin();
			 eit != SpriteManager::GetSingleton().GetTypeList("enemy").end();
			 eit++) {
			auto r = ((const BoxCollider2D*)(*eit)->GetCollider())->GetRect();
			tile = Image::Create(r.width, r.height, {0, 0, 255, 255});
			tile->BlitScaled({0, 0, r.width, r.height}, *map->GetTilemap(), r);
			delete tile;
		}

		auto dit = SpriteManager::GetSingleton().GetTypeList("door").begin();
		if (dit != SpriteManager::GetSingleton().GetTypeList("door").end()) {
			tile = Image::Create(16, 48, {0, 0, 255, 255});
			tile->BlitScaled(
				{0, 0, 16, 32}, *map->GetTilemap(),
				((const BoxCollider2D*)(*dit)->GetCollider())->GetRect());
			delete tile;
		}

		auto git = SpriteManager::GetSingleton().GetTypeList("gate").begin();
		if (git != SpriteManager::GetSingleton().GetTypeList("gate").end()) {
			tile = Image::Create(16, 48, {0, 0, 255, 255});
			tile->BlitScaled(
				{0, 0, 16, 32}, *map->GetTilemap(),
				((const BoxCollider2D*)(*git)->GetCollider())->GetRect());
			delete tile;
		}

		tile = Image::Create(16, 16, {0, 0, 255, 255});
		for (auto lit =
				 SpriteManager::GetSingleton().GetTypeList("link.attacker").begin();
			 lit != SpriteManager::GetSingleton().GetTypeList("link.attacker").end();
			 ++lit) {
			tile->BlitScaled(
				{0, 0, 16, 16}, *map->GetTilemap(),
				((const BoxCollider2D*)(*lit)->GetCollider())->GetRect());
		}
		delete tile;

		tile = Image::Create(8, 16, {0, 0, 255, 255});
		for (auto iit =
				 SpriteManager::GetSingleton().GetTypeList("item").begin();
			 iit != SpriteManager::GetSingleton().GetTypeList("item").end();
			 ++iit) {
			tile->BlitScaled(
				{0, 0, 8, 16}, *map->GetTilemap(),
				((const BoxCollider2D*)(*iit)->GetCollider())->GetRect());
		}
		delete tile;
#endif	// DEBUG

		Clipper clipper;
		clipper.SetView([&]() { return map->GetView(); });

		auto& dpyList = SpriteManager::GetSingleton().GetDisplayList();
		for (auto* sprite : dpyList) {
			if (sprite->IsVisible()) {
				sprite->Display(displayBuffer,
								{0, 0, displayBuffer->GetWidth(),
								 displayBuffer->GetHeight()},
								clipper);
			}
		}

		Texture tex(renderer->GetRenderer(), *displayBuffer);

		renderer->Clear();
		renderer->Copy(
			tex, {0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()},
			{0, 0, window->GetWidth(), window->GetHeight()});

		ui->Render(*renderer);

		renderer->Render();
	}
	
	virtual void Input() override {
		using namespace GameEngine;
		using namespace GameEngine::Graphics;

		if (Input::GetKeyDown(Event::Ecsape)) {
			if (!this->IsPaused()) {
				this->Pause();
			} else {
				this->Resume();
			}
		}
	}

	virtual void ProgressAnimations() override {
		GameEngine::AnimatorManager::GetSingleton().Progress(GameEngine::Time::getTime());
	}

	virtual void CommitDestructions() override {
		GameEngine::CollisionChecker::GetSingleton().RemoveDead();
		GameEngine::DestructionManager::Get().Commit();
	}

	virtual void CollisionChecking() override {
		GameEngine::CollisionChecker::GetSingleton().Check();
	}

	virtual void AI() override {
		auto& enemylist =
			GameEngine::SpriteManager::GetSingleton().GetTypeList("enemy");
		for (auto enemy = enemylist.begin(); enemy != enemylist.end(); enemy++)
			((Enemy*)(*enemy))->Start();
	}

	virtual void UserCode() override {
		using namespace GameEngine;

		auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
		Player* player =
			(it != SpriteManager::GetSingleton().GetTypeList("player").end())
				? (Player*)*it
				: nullptr;
		if (player)
			player->Update();

		if (Input::GetMouseButtonDown(Input::MouseMiddle)) {
			if (camera->GetFollow() == nullptr && player)
				camera->Follow(&player->position);
			else
				camera->Follow(nullptr);
		}

		if (Input::GetMouse(Input::MouseLeft) && canScroll) {
			auto ms = Input::GetMouseState();
			int dx = 0, dy = 0;
			int x = ms.mouseX - window->GetWidth() / 2;
			int y = ms.mouseY - window->GetHeight() / 2;
			dx = scrollSpeed * Math::Sign(x);
			dy = scrollSpeed * Math::Sign(y);

			camera->Scroll(dx, dy);
		}

		if (elevator)
			elevator->CallElevator();

		if (elevator3)
			elevator3->CallElevator();

		if (elevator2)
			elevator2->CallElevator();

		auto door = SpriteManager::GetSingleton().GetTypeList("door").begin();
		if (door != SpriteManager::GetSingleton().GetTypeList("door").end() &&
			(*door)->IsVisible())
			((Door*)*door)->Unlock();

		auto gate = SpriteManager::GetSingleton().GetTypeList("gate").begin();
		if (gate != SpriteManager::GetSingleton().GetTypeList("gate").end() &&
			(*gate)->IsVisible())
			((Door*)*gate)->Unlock();
	}

};