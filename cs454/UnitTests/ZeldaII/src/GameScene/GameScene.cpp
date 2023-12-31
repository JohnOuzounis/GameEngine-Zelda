#include "GameScene.h"
#include <GameEngine/BoxCollider2D.h>

int Stalfos::attackId = 0;

using namespace GameEngine;
using namespace GameEngine::Graphics;
using namespace GameEngine::Json;

void GameScene::LoadMaps() {
	appConfig.Configure(Resources::Get().GetResource("config/app.json"));

	std::string mapName = ((StringProperty*)appConfig.GetConfigurations()
							   ->GetProperties()
							   .find("map")
							   ->second)
							  ->GetValueStripped();

	Configurator mapConfig;
	mapConfig.Configure(Resources::Get().GetResource(mapName));

	AggregateProperty* tilemap =
		(AggregateProperty*)mapConfig.GetConfigurations()
			->GetProperties()
			.find("tilemap")
			->second;

	TilemapMaker actionMaker;
	actionMaker.Configure(*tilemap);
	map = actionMaker.MakeTilemap();
	map->SetView(
		{1250, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()});

	AggregateProperty* gridConfig =
		(AggregateProperty*)mapConfig.GetConfigurations()
			->GetProperties()
			.find("gridmap")
			->second;

	int gridWidth = (int)((NumericProperty*)gridConfig->GetProperties()
							  .find("tilewidth")
							  ->second)
						->GetValue();
	int gridHeight = (int)((NumericProperty*)gridConfig->GetProperties()
							   .find("tileheight")
							   ->second)
						 ->GetValue();

	this->map->SetGridmap(gridWidth, gridHeight);

	Gridmap* grid = map->GetGridmap();
	ArrayProperty* array =
		(ArrayProperty*)gridConfig->GetProperties().find("map")->second;
	assert(grid->GetTotal() == array->GetSize());

	Gridmap::GridIndex* gridmap = grid->GetGrid();
	for (int i = 0; i < grid->GetTotal(); i++) {
		gridmap[i] =
			(Gridmap::GridIndex)((NumericProperty*)array->Get(i))->GetValue();
	}
}

static void LoadFilm(Configurator& appConfig,
			  std::list<AnimationFilm*>& list,
			  std::vector<std::string> path,
			  const std::string& bitmap,
			  const Point& trans = {0, 0}) {
	AggregateProperty* obj = (AggregateProperty*)appConfig.GetConfigurations()
								 ->GetProperties()
								 .find(path[0])
								 ->second;
	for (int i = 1; i < path.size(); i++) {
		obj = (AggregateProperty*)obj->GetProperties().find(path[i])->second;
	}

	AggregateProperty::Properties& properties =
		((AggregateProperty*)obj->GetProperties().find("animations")->second)
			->GetProperties();

	for (auto& prop : properties) {
		AnimationFilm* film = new AnimationFilm(prop.first);
		ArrayProperty* frames = (ArrayProperty*)prop.second;
		for (auto* frame : frames->GetProperties()) {
			AggregateProperty::Properties& f =
				((AggregateProperty*)frame)->GetProperties();
			Rect r = {
				(int)((NumericProperty*)f.find("x")->second)->GetValue(),
				(int)((NumericProperty*)f.find("y")->second)->GetValue(),
				(int)((NumericProperty*)f.find("width")->second)->GetValue(),
				(int)((NumericProperty*)f.find("height")->second)->GetValue()};

			Point offset = {0, 0};
			auto it = f.find("offset");
			if (it != f.end()) {
				offset.x =
					(int)((NumericProperty*)((AggregateProperty*)it->second)
							  ->GetProperties()
							  .find("x")
							  ->second)
						->GetValue();
				offset.y =
					(int)((NumericProperty*)((AggregateProperty*)it->second)
							  ->GetProperties()
							  .find("y")
							  ->second)
						->GetValue();
			}

			Rect collider = {0, 0, 0, 0};
			it = f.find("collider");
			if (it != f.end()) {
				collider.x =
					(int)((NumericProperty*)((AggregateProperty*)it->second)
							  ->GetProperties()
							  .find("x")
							  ->second)
						->GetValue();
				collider.y =
					(int)((NumericProperty*)((AggregateProperty*)it->second)
							  ->GetProperties()
							  .find("y")
							  ->second)
						->GetValue();
				collider.width =
					(int)((NumericProperty*)((AggregateProperty*)it->second)
							  ->GetProperties()
							  .find("width")
							  ->second)
						->GetValue();
				collider.height =
					(int)((NumericProperty*)((AggregateProperty*)it->second)
							  ->GetProperties()
							  .find("height")
							  ->second)
						->GetValue();
			}

			film->Append(r, offset, collider);
		}
		film->SetBitmap(ImageLoader::GetImageLoader().Load(
			Resources::Get().GetAsset(bitmap)));
		if (trans.x != 0 && trans.y != 0) {
			Color* color = film->GetBitmap()->GetColorAt(trans);
			film->GetBitmap()->SetColorKey(*color, true);
			delete color;
		}

		list.push_back(film);
	}
}

void GameScene::LoadFilms() {
	AnimationFilmHolder::Get().Load(
		Resources::Get().GetResource("config/app.json"),
		[](std::list<AnimationFilm*>& list, const std::string& filepath) {
			Configurator appConfig;
			appConfig.Configure(filepath);

			LoadFilm(appConfig, list, {"link"}, "bitmaps/link-sprites.png");
			LoadFilm(appConfig, list, {"gamescene", "elevator"},
					 "bitmaps/Zelda-II-Parapa-Palace-Tileset.png");
			LoadFilm(appConfig, list, {"gamescene", "door"},
					 "bitmaps/items-objects-sprites.png", {1, 11});
			LoadFilm(appConfig, list, {"gamescene", "items"},
					 "bitmaps/items-objects-sprites.png", {1, 11});
			LoadFilm(appConfig, list, {"gamescene", "enemies"},
					 "bitmaps/basic-enemies-sprites.png");

			return true;
		});
}

void GameScene::LoadAreaBounds() {
	auto areas =
		(ArrayProperty*)((AggregateProperty*)appConfig.GetConfigurations()
							 ->GetProperties()
							 .find("gamescene")
							 ->second)
			->GetProperties()
			.find("areas")
			->second;

	for (auto area : areas->GetProperties()) {
		int x = (int)((NumericProperty*)((AggregateProperty*)area)
						  ->GetProperties()
						  .find("x")
						  ->second)
					->GetValue();
		int y = (int)((NumericProperty*)((AggregateProperty*)area)
						  ->GetProperties()
						  .find("y")
						  ->second)
					->GetValue();
		int width = (int)((NumericProperty*)((AggregateProperty*)area)
							  ->GetProperties()
							  .find("width")
							  ->second)
						->GetValue();
		int height = (int)((NumericProperty*)((AggregateProperty*)area)
							   ->GetProperties()
							   .find("height")
							   ->second)
						 ->GetValue();
		this->areas.push_back({x, y, width, height});
	}
}

void GameScene::LoadVars() {
	scrollSpeed = (int)((NumericProperty*)((AggregateProperty*)appConfig
											   .GetConfigurations()
											   ->GetProperties()
											   .find("gamescene")
											   ->second)
							->GetProperties()
							.find("scrollspeed")
							->second)
					  ->GetValue();
	lavaDmg = (int)((NumericProperty*)((AggregateProperty*)appConfig
										   .GetConfigurations()
										   ->GetProperties()
										   .find("gamescene")
										   ->second)
						->GetProperties()
						.find("lavadamage")
						->second)
				  ->GetValue();

	sfxVol = (int)((NumericProperty*)((AggregateProperty*)appConfig
										  .GetConfigurations()
										  ->GetProperties()
										  .find("gamescene")
										  ->second)
					   ->GetProperties()
					   .find("sfxvol")
					   ->second)
				 ->GetValue();
}

void GameScene::LoadAudio() {
	AudioManager::Get().SetSoundEffectVolume(sfxVol);

	AudioManager::Get().GetAudio("audio/boss_battle.wav")->SetLoop(true);
	AudioManager::Get().GetAudio("audio/battle_area.wav")->SetLoop(true);
}

void GameScene::MakeLava(int x, int y, int w, int dmg) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	if (!player)
		return;

	Sprite* lava = new Sprite(x, y, nullptr, "lava");
	lava->SetVisibility(false);
	lava->SetCollider(new BoxCollider2D(x, y, w, 16, "lava"));
	CollisionChecker::GetSingleton().Register(
		player, lava,
		[dmg](Sprite* s1, Sprite* s2) { ((Player*)s1)->TakeDamage(dmg); });
}

template <typename T>
T* GameScene::MakeCharacter(int x,
			 int y,
			 const std::string& configName,
			 GameEngine::AnimationFilm* film) {
	AggregateProperty* speed =
		(AggregateProperty*)((AggregateProperty*)appConfig.GetConfigurations()
								 ->GetProperties()
								 .find(configName)
								 ->second)
			->GetProperties()
			.find("speed")
			->second;
	float xvelocity = (float)((NumericProperty*)speed->GetProperties()
								  .find("xvelocity")
								  ->second)
						  ->GetValue();

	float yvelocity = (float)((NumericProperty*)speed->GetProperties()
								  .find("yvelocity")
								  ->second)
						  ->GetValue();

	int dx = (int)((NumericProperty*)speed->GetProperties().find("dx")->second)
				 ->GetValue();

	int dy = (int)((NumericProperty*)speed->GetProperties().find("dy")->second)
				 ->GetValue();

	AggregateProperty* health =
		(AggregateProperty*)((AggregateProperty*)appConfig.GetConfigurations()
								 ->GetProperties()
								 .find(configName)
								 ->second)
			->GetProperties()
			.find("health")
			->second;
	int bars = (int)((NumericProperty*)health->GetProperties()
						 .find("healthbars")
						 ->second)
				   ->GetValue();

	int healthPerBar = (int)((NumericProperty*)health->GetProperties()
								 .find("healthperbar")
								 ->second)
						   ->GetValue();

	int dmg = (int)((NumericProperty*)((AggregateProperty*)appConfig
										   .GetConfigurations()
										   ->GetProperties()
										   .find(configName)
										   ->second)
						->GetProperties()
						.find("damage")
						->second)
				  ->GetValue();

	T* character = new T(x, y, dx, dy, xvelocity, yvelocity, film);

	character->SetHealth(bars, healthPerBar);
	character->SetDamage(dmg);
	character->SetCharacterController(*map->GetGridmap());
	auto r = character->GetFilm()->GetCollider(character->GetFrame());
	character->SetCharacterPosition(r.x, r.y, r.width, r.height);

	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;
	if (player)
		CollisionChecker::GetSingleton().Register(
			player, character, [](Sprite* s1, Sprite* s2) {
				if (!((Player*)s1)
						 ->CheckBlockedAttack(
							 ((BoxCollider2D*)s2->GetCollider())->GetRect()))

					((Player*)s1)->TakeDamage(((Wosu*)s2)->Attack());
			});

	return character;
}

Wosu* GameScene::MakeWosu(int x, int y, GameEngine::AnimationFilm* film) {
	Wosu* wosu = MakeCharacter<Wosu>(x, y, "wosu", film);
	wosu->SetOnDeath([&, wosu]() {
		if (Math::Random(0, 100) <= 30)
			MakePointbag(
				((BoxCollider2D*)wosu->GetCollider())->GetX(),
				((BoxCollider2D*)wosu->GetCollider())->GetY() +
					((BoxCollider2D*)wosu->GetCollider())->GetHeight() - 16,
				Math::Random(1, 5));
	});
	wosu->SetOnTakeDamage(
		[]() { AudioManager::Get().PlayEffect("audio/hit_enemy.wav");
	});

	return wosu;
}

Stalfos* GameScene::MakeStalfos(int x, int y, GameEngine::AnimationFilm* film) {
	AggregateProperty* aoe =
		(AggregateProperty*)((AggregateProperty*)appConfig.GetConfigurations()
								 ->GetProperties()
								 .find("stalfos")
								 ->second)
			->GetProperties()
			.find("aoe")
			->second;

	int w = (int)((NumericProperty*)aoe->GetProperties().find("width")->second)
				->GetValue();

	int h = (int)((NumericProperty*)aoe->GetProperties().find("height")->second)
				->GetValue();

	Stalfos* stalfos = MakeCharacter<Stalfos>(x, y, "stalfos", film);
	stalfos->SetOnDeath([&, stalfos]() {
		MakePointbag(((BoxCollider2D*)stalfos->GetCollider())->GetX(),
					 ((BoxCollider2D*)stalfos->GetCollider())->GetY(), 30);
	});
	stalfos->SetOnAttack([stalfos](Sprite* s1, Sprite* s2) {
		if (s2 && s2->IsAlive()) {
			if (!((Player*)s2)
					 ->CheckBlockedAttack(
						 ((BoxCollider2D*)s1->GetCollider())->GetRect()))
				((Player*)s2)->TakeDamage(stalfos->GetDamage());
			if (s1->IsAlive()) {
				delete s1->GetCollider();
				s1->Destroy();
			}
		}
	});
	stalfos->SetOnTakeDamage(
		[]() { AudioManager::Get().PlayEffect("audio/hit_enemy.wav"); });
	stalfos->SetAreaOfEffect(w, h);

	return stalfos;
}

Bot* GameScene::MakeBot(int x, int y) {
	AggregateProperty* aoe =
		(AggregateProperty*)((AggregateProperty*)appConfig.GetConfigurations()
								 ->GetProperties()
								 .find("bot")
								 ->second)
			->GetProperties()
			.find("aoe")
			->second;

	int w = (int)((NumericProperty*)aoe->GetProperties().find("width")->second)
				->GetValue();

	int h = (int)((NumericProperty*)aoe->GetProperties().find("height")->second)
				->GetValue();

	Bot* bot = MakeCharacter<Bot>(
		x, y, "bot",
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("bot.idle"));
	bot->SetOnDeath([&, bot]() {
		MakePointbag(((BoxCollider2D*)bot->GetCollider())->GetX(),
					 ((BoxCollider2D*)bot->GetCollider())->GetY() +
						 ((BoxCollider2D*)bot->GetCollider())->GetHeight() - 16,
					 10);
	});
	bot->SetOnTakeDamage(
		[]() { AudioManager::Get().PlayEffect("audio/hit_enemy.wav"); });

	bot->SetAreaOfEffect(w, h);

	return bot;
}

Player* GameScene::MakePlayer() {
	AggregateProperty* speed =
		(AggregateProperty*)((AggregateProperty*)appConfig.GetConfigurations()
								 ->GetProperties()
								 .find("link")
								 ->second)
			->GetProperties()
			.find("speed")
			->second;
	float xvelocity = (float)((NumericProperty*)speed->GetProperties()
								  .find("xvelocity")
								  ->second)
						  ->GetValue();

	float yvelocity = (float)((NumericProperty*)speed->GetProperties()
								  .find("yvelocity")
								  ->second)
						  ->GetValue();

	int dx = (int)((NumericProperty*)speed->GetProperties().find("dx")->second)
				 ->GetValue();

	int dy = (int)((NumericProperty*)speed->GetProperties().find("dy")->second)
				 ->GetValue();

	AggregateProperty* health =
		(AggregateProperty*)((AggregateProperty*)appConfig.GetConfigurations()
								 ->GetProperties()
								 .find("link")
								 ->second)
			->GetProperties()
			.find("health")
			->second;
	int bars = (int)((NumericProperty*)health->GetProperties()
						 .find("healthbars")
						 ->second)
				   ->GetValue();

	int healthPerBar = (int)((NumericProperty*)health->GetProperties()
								 .find("healthperbar")
								 ->second)
						   ->GetValue();

	int dmg = (int)((NumericProperty*)((AggregateProperty*)appConfig
										   .GetConfigurations()
										   ->GetProperties()
										   .find("link")
										   ->second)
						->GetProperties()
						.find("damage")
						->second)
				  ->GetValue();

	Player* player = new Player(
		1280, 170, xvelocity, yvelocity, dx, dy,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("link.idle.right"));

	player->SetHealth(bars, healthPerBar);
	player->SetDamage(dmg);

	player->SetCharacterController(*map->GetGridmap());

	auto r = player->GetFilm()->GetCollider(player->GetFrame());
	player->SetCharacterPosition(r.x, r.y, r.width, r.height);
	player->Idle();

	return player;
}

GameEngine::Camera* GameScene::MakeCamera() {
	GameEngine::Camera* cam = new GameEngine::Camera(
		displayBuffer->GetWidth(), displayBuffer->GetHeight());

	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	if (player) {
		cam->Follow(&player->position);
	}
	cam->SetOffset(displayBuffer->GetWidth() / 2,
				   displayBuffer->GetHeight() / 2);

	return cam;
}

Elevator* GameScene::MakeElevator(int x,
								  int yTop,
								  int yBot,
								  int dx,
								  int dy,
								  int reps,
								  double delay,
								  std::string topTag,
								  std::string botTag) {
	Sprite* top = new Sprite(
		x, yTop,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("elevator.idle"),
		topTag);
	top->SetVisibility(true);

	Sprite* bot = new Sprite(
		x, yBot,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("elevator.idle"),
		botTag);
	bot->SetVisibility(true);

	Elevator* el = new Elevator(top, bot, false, dx, dy);
	el->SetUpdater([&](const Rect& position, Gridmap::GridIndex val) {
		int startRow = position.y / map->GetGridmap()->GetGridTileHeight();
		int endRow = (position.y + position.height) /
						 map->GetGridmap()->GetGridTileHeight() -
					 1;

		int startCol = position.x / map->GetGridmap()->GetGridTileWidth();
		int endCol = (position.x + position.width) /
						 map->GetGridmap()->GetGridTileWidth() -
					 1;

		map->GetGridmap()->SetGridTileBlock(startRow, endRow, startCol, endCol,
											val);
	});
	el->Update(15);

	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	el->SetOnStartMoving([&, player]() {
		if (player) {
			player->canMove = false;
			player->GetGravityHandler().SetGravity(false);
		}
		this->canScroll = false;
	});
	el->SetOnMove([&, player](int dx, int dy) {
		if (player)
			player->MoveCharacter(dx, dy);
	});
	el->SetOnStopMoving([&, player]() {
		if (player) {
			player->canMove = true;
			player->GetGravityHandler().SetGravity(true);
		}
		this->canScroll = true;
	});

	CollisionChecker::GetSingleton().Register(
		player, el->bot, [el, reps, delay](Sprite* s1, Sprite* s2) {
			if (!s1)
				return;

			assert(s1->GetTypeId() == "player");
			Player* player = (Player*)s1;

			if (!player->IsAlive() || !el)
				return;

			if (el->called && el->animator->HasFinished()) {
				BoxCollider2D box = *(BoxCollider2D*)s2->GetCollider();

				if (box.OverlapPoint(
						{player->position.x,
						 player->position.y + player->position.height}) &&
					box.OverlapPoint(
						{player->position.x + player->position.width,
						 player->position.y + player->position.height})) {
					el->Move(reps, delay);
				}
			}
		});

	return el;
}

const GameEngine::Graphics::Rect& GameScene::GetArea(
	const GameEngine::Graphics::Rect& position) {
	for (auto& rect : areas) {
		BoxCollider2D area(rect.x, rect.y, rect.width, rect.height, "area");

		if (area.OverlapPoint(position.x, position.y))
			return rect;
	}
	assert(false);
}

void GameScene::MakeDoll(int x, int y) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	MakeItem(x, y, "doll.idle", "doll")->SetEffect([player](Item* item) {
		if (player) {
			player->health.AddHealthBar();
			AudioManager::Get().PlayEffect("audio/item_collect2.wav");
		}
	});
}

void GameScene::MakeGauntlet(int x, int y, int dmg) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	MakeItem(x, y, "gauntlet.idle", "gauntlet")
		->SetEffect([player, dmg](Item* item) {
			if (player) {
				player->SetDamage(player->GetDamage() + dmg);
				AudioManager::Get().PlayEffect("audio/powerup.wav");
			}
		});
}

void GameScene::MakePointbag(int x, int y, int points) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	MakeItem(x, y, "pointbag.idle", "pointbag")
		->SetEffect([player, points](Item* item) {
			if (!player || !player->IsAlive())
				return;

			player->scorePoints += points;
			AudioManager::Get().PlayEffect("audio/pointbag_collect.wav");
		});
}

void GameScene::MakeHealPotion(int x, int y) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	MakeItem(x, y, "healpotion.idle", "healpotion")
		->SetEffect([player](Item* item) {
			if (!player || !player->IsAlive())
				return;

			while (!player->health.IsFull())
				player->health.Heal(player->health.GetHealthPerBar());
			AudioManager::Get().PlayEffect("audio/item_collect2.wav");
		});
}

void GameScene::MakeHeart(int x, int y) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	MakeItem(x, y, "heart.idle", "heart")->SetEffect([player](Item* item) {
		if (!player || !player->IsAlive())
			return;

		player->health.Heal(player->health.GetHealthPerBar());
		AudioManager::Get().PlayEffect("audio/item_collect2.wav");
	});
}

void GameScene::MakePotion(int x,
						   int y,
						   const std::string& film,
						   const std::string& tag,
						   int points) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	MakeItem(x, y, film, tag)->SetEffect([player, points](Item* item) {
		if (!player || !player->IsAlive())
			return;

		player->magicPoints += points;
		AudioManager::Get().PlayEffect("audio/powerup.wav");
	});
}

void GameScene::MakeKey(int x,
						int y,
						const std::string& film,
						const std::string& tag) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	MakeItem(x, y, film, tag)->SetEffect([player](Item* item) {
		if (!player || !player->IsAlive())
			return;
		AudioManager::Get().PlayEffect("audio/item_collect.wav");
	});
}

Door* GameScene::MakeDoor() {
	Door* d = new Door(
		880, 688,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("door.idle"),
		"door");
	d->SetUpdater([&](const Rect& position, Gridmap::GridIndex val) {
		int startRow = position.y / map->GetGridmap()->GetGridTileHeight();
		int endRow = (position.y + position.height) /
						 map->GetGridmap()->GetGridTileHeight() -
					 1;

		int startCol = position.x / map->GetGridmap()->GetGridTileWidth();
		int endCol = (position.x + position.width) /
						 map->GetGridmap()->GetGridTileWidth() -
					 1;

		map->GetGridmap()->SetGridTileBlock(startRow, endRow, startCol, endCol,
											val);
	});
	d->Update(GRID_SOLID_TILE);

	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	CollisionChecker::GetSingleton().Register(
		player, d, [](Sprite* s1, Sprite* s2) {
			if (((Player*)s1)->HasItem("key"))
				((Door*)s2)->OpenDoor();

			if (!s2->IsAlive())
				CollisionChecker::GetSingleton().Cancel(s1, s2);
		});

	return d;
}

Door* GameScene::MakeGate() {
	Door* d = new Door(
		3280 - 16, 960,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("door.idle"),
		"gate");
	d->SetUpdater([&](const Rect& position, Gridmap::GridIndex val) {
		int startRow = position.y / map->GetGridmap()->GetGridTileHeight();
		int endRow = (position.y + position.height) /
						 map->GetGridmap()->GetGridTileHeight() -
					 1;

		int startCol = position.x / map->GetGridmap()->GetGridTileWidth();
		int endCol = (position.x + position.width) /
						 map->GetGridmap()->GetGridTileWidth() -
					 1;

		map->GetGridmap()->SetGridTileBlock(startRow, endRow, startCol, endCol,
											val);
	});
	d->SetVisibility(false);
	((BoxCollider2D*)d->GetCollider())
		->SetX(((BoxCollider2D*)d->GetCollider())->GetX() + 32);

	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	CollisionChecker::GetSingleton().Register(
		player, d, [](Sprite* s1, Sprite* s2) {
			if (!s2->IsVisible()) {
				((BoxCollider2D*)((Door*)s2)->GetCollider())
					->SetX(
						((BoxCollider2D*)((Door*)s2)->GetCollider())->GetX() -
						32);
				((Door*)s2)->CloseDoor();
			} else {
				if (((Player*)s1)->HasItem("masterkey"))
					((Door*)s2)->OpenDoor();

				if (!s2->IsAlive())
					CollisionChecker::GetSingleton().Cancel(s1, s2);
			}
		});

	return d;
}

Item* GameScene::MakeItem(int x,
						  int y,
						  const std::string& film,
						  const std::string& tag) {
	auto it = SpriteManager::GetSingleton().GetTypeList("player").begin();
	Player* player =
		(it != SpriteManager::GetSingleton().GetTypeList("player").end())
			? (Player*)*it
			: nullptr;

	Item* item = new Item(
		x, y, (AnimationFilm*)AnimationFilmHolder::Get().GetFilm(film), tag);
	return item;
}

void GameScene::SpawnEnemies() {
	MakeWosu(1072, 720,
			 (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("wosu.right"));
	MakeWosu(1216, 720,
			 (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("wosu.left"));
	MakeWosu(1312, 720,
			 (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("wosu.right"));
	MakeWosu(1936, 720,
			 (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("wosu.left"));
	MakeWosu(1430, 150,
			 (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("wosu.right"));
	MakeWosu(1952, 624,
			 (AnimationFilm*)AnimationFilmHolder::Get().GetFilm("wosu.left"));

	MakeBot(1376, 480);
	MakeBot(1264, 480);
	MakeBot(160, 480);
	MakeBot(1120, 480);
	MakeBot(2096, 480);
	MakeBot(2176, 480);
	MakeBot(2560, 672);
	MakeBot(1952, 560);
	MakeBot(3360, 992);

	MakeStalfos(
		288, 288,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("stalfos.left"));
	MakeStalfos(
		592, 288,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("stalfos.right"));
}

void GameScene::SpawnItems() {
	MakeKey(48, 464, "key.idle", "key");
	MakeKey(4432, 992, "masterkey.idle", "masterkey");
	MakePotion(48, 352, "redpotion.idle", "redpotion", 128);
	MakePointbag(752, 704, 10);
	MakePointbag(768, 704, 10);
	MakePotion(720, 688, "bluepotion.idle", "bluepotion", 16);
	MakeGauntlet(736, 704, 8);
	MakeDoll(3120, 704);
	MakeHealPotion(4400, 992);
	MakeHeart(2832, 480);
}
