#include "Unit3.h"
#include <GameEngine/Animation/AnimatorManager.h>
#include <GameEngine/Animation/FrameRangeAnimator.h>
#include <GameEngine/Animation/FrameListAnimator.h>
#include <GameEngine/Animation/FlashAnimator.h>
#include <GameEngine/Animation/ScrollAnimator.h>
#include <GameEngine/Animation/MovingAnimator.h>
#include <GameEngine/Animation/TickAnimator.h>
#include <GameEngine/Graphics/Texture.h>
#include <GameEngine/Input.h>
#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/Configurator.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <GameEngine/JSON/StringProperty.h>
#include <GameEngine/LatelyDestroyable.h>
#include <GameEngine/Resources.h>
#include <GameEngine/System.h>
#include <GameEngine/Time.h>
#include "../../Unit1/src/TilemapMaker.h"

using namespace GameEngine;
using namespace GameEngine::Json;
using namespace GameEngine::Graphics;

void Unit3::Initialise() {
	System::Init();

	Resources::Get().SetAssetPath(
		"../../../../../../cs454/UnitTests/Unit3/assets/");
	Resources::Get().SetRootPath("../../../../../../cs454/UnitTests/Unit3/");
}

void Unit3::Load() {
	window = new Window("Unit3", 100, 100, 680, 680, Window::Flags::Shown);
	renderer = new Renderer(*window, -1, Renderer::Accelerated);
	displayBuffer = Image::Create(240, 240, {0, 0, 0, 255});

	this->game.SetFinished([&]() { return !window->IsOpen(); });
	this->game.AddQuit([&]() { window->Close(); }, false);
	this->game.AddAnim(
		[]() { AnimatorManager::GetSingleton().Progress(Time::getTime()); },
		false);
	this->game.AddDestruct([&]() { DestructionManager::Get().Commit(); },
						   false);

	this->game.AddRender(
		[&]() {
			displayBuffer->Clear({0, 0, 0, 255});

			bg->Display(displayBuffer, {0, 0, displayBuffer->GetWidth(),
										displayBuffer->GetHeight()});

			actionTilemap->Display(
				*displayBuffer,
				{0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()});

			Clipper clipper;
			clipper.SetView([&]() { return actionTilemap->GetView(); });

			if (link != nullptr && link->IsVisible()) {
				link->Display(displayBuffer,
							  {0, 0, displayBuffer->GetWidth(),
							   displayBuffer->GetHeight()},
							  clipper);
			}

			Texture tex(renderer->GetRenderer(), *displayBuffer);
			renderer->Clear();
			renderer->Copy(
				tex,
				{0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()},
				{0, 0, window->GetWidth(), window->GetHeight()});
			renderer->Render();
		},
		false);

	this->game.AddInput(
		[&]() {
			int dx = 0, dy = 0;
			if (Input::GetKeyDown(Event::Left)) {
				if (anim->HasFinished())
					anim->Start(a, Time::getTime());
				dx = -1;
			}
			if (Input::GetKeyUp(Event::Left))
				anim->Stop();

			if (Input::GetKeyDown(Event::Right)) {
				if (anim->HasFinished())
					anim->Start(b, Time::getTime());
				dx = 1;
			}
			if (Input::GetKeyUp(Event::Right))
				anim->Stop();

			if (Input::GetKeyDown(Event::Up)) {
				if (m->HasFinished())
					m->Start(c, Time::getTime());
				dy = -1;
			}
			if (Input::GetKeyDown(Event::Down))
				dy = 1;

			if (actionTilemap->CanScrollHorizontal(2 * dx)) {
				bg->Scroll(dx);
				actionTilemap->Scroll(2 * dx, 2 * dy);
			}

			if (Input::GetKeyDown(Event::K))
				fla->Start(fl, Time::getTime());
		},
		false);

	Configurator appConfig;
	appConfig.Configure(Resources::Get().GetResource("config/app.json"));

	std::string mapName = ((StringProperty*)appConfig.GetConfigurations()
							   ->GetProperties()
							   .find("map")
							   ->second)
							  ->GetValueStripped();

	AnimationFilmHolder::Get().Load(
		Resources::Get().GetResource("config/app.json"),
		[](std::list<AnimationFilm*>& list, const std::string& filepath) {
			Configurator appConfig;
			appConfig.Configure(filepath);

			AggregateProperty::Properties& link =
				((AggregateProperty*)((AggregateProperty*)appConfig
										  .GetConfigurations()
										  ->GetProperties()
										  .find("link")
										  ->second)
					 ->GetProperties()
					 .find("animations")
					 ->second)
					->GetProperties();

			for (auto& prop : link) {
				AnimationFilm* film = new AnimationFilm(prop.first);
				ArrayProperty* frames = (ArrayProperty*)prop.second;
				for (auto* frame : frames->GetProperties()) {
					AggregateProperty::Properties& f =
						((AggregateProperty*)frame)->GetProperties();
					Rect r = {(int)((NumericProperty*)f.find("x")->second)
								  ->GetValue(),
							  (int)((NumericProperty*)f.find("y")->second)
								  ->GetValue(),
							  (int)((NumericProperty*)f.find("width")->second)
								  ->GetValue(),
							  (int)((NumericProperty*)f.find("height")->second)
								  ->GetValue()};

					Point offset = {0, 0};
					auto it = f.find("offset");
					if (it != f.end()) {
						offset.x = (int)((NumericProperty*)((AggregateProperty*)
																it->second)
											 ->GetProperties()
											 .find("x")
											 ->second)
									   ->GetValue();
						offset.y = (int)((NumericProperty*)((AggregateProperty*)
																it->second)
											 ->GetProperties()
											 .find("y")
											 ->second)
									   ->GetValue();
					}
					film->Append(r, offset, {0,0,0,0});
				}
				film->SetBitmap(ImageLoader::GetImageLoader().Load(
					Resources::Get().GetAsset("bitmaps/link-sprites.png")));
				list.push_back(film);
			}

			return true;
		});

	link = new Sprite(
		150, 190,
		(AnimationFilm*)AnimationFilmHolder::Get().GetFilm("link.idle.right"),
		"player");
	link->SetOnDestroy([&]() { link = nullptr; });
	link->SetVisibility(true);

	FrameListAnimation::Frames f;
	f.push_back(3);
	f.push_back(2);
	f.push_back(1);
	f.push_back(0);
	a = new FrameListAnimation("link.walk.left", f, 0,-2,0,0.1);

	FrameListAnimation::Frames ff;
	ff.push_back(0);
	ff.push_back(1);
	ff.push_back(2);
	ff.push_back(3);
	b = new FrameListAnimation("link.walk.right", ff, 0, 2, 0, 0.1);

	anim = new FrameListAnimator();
	anim->SetOnAction([&](Animator* anim, const Animation& a) {
		link->SetHasDirectMotion(true);
		FrameListAnimation* b = (FrameListAnimation*)&a;
		link->Move(b->GetDx(), b->GetDy());

		link->SetFilm(
			(AnimationFilm*)AnimationFilmHolder::Get().GetFilm(a.GetId()));
		link->SetFrame(((FrameListAnimator*)anim)->GetCurrFrame());
	});

	c = new MovingAnimation("jump", 4, 0, -8, 0.05);
	m = new MovingAnimator();
	m->SetOnAction([&](Animator* anim, const Animation& a) {
		link->SetHasDirectMotion(true);
		MovingAnimation* b = (MovingAnimation*)&a;
		link->Move(b->GetDx(), b->GetDy());
	});

	fl = new FlashAnimation("hit", 4, 0.1, 0.2);
	fla = new FlashAnimator();
	fla->SetOnAction([&](Animator* anim, const Animation& a) {
		link->SetVisibility(!link->IsVisible());
	});

	ScrollAnimation* sc = new ScrollAnimation("scroll", {{1, 0, 1},
														 {2, 0, 1},
														 {3, 0, 1},
														 {4, 0, 1},
														 {5, 0, 1},
														 {6, 0, 1},
														 {7, 0, 1}});
	ScrollAnimator* sca = new ScrollAnimator();
	sca->SetOnAction([&](Animator* anim, const Animation& a) {
		int dx = ((ScrollAnimation*)&a)
					 ->GetScroll()[((ScrollAnimator*)anim)->GetCurrIndex()]
					 .dx;
		int dy = ((ScrollAnimation*)&a)
					 ->GetScroll()[((ScrollAnimator*)anim)->GetCurrIndex()]
					 .dy;
		std::cout << dx << " " << dy << "\n";
		if (actionTilemap->CanScrollHorizontal(2 * dx)) {
			bg->Scroll(dx);
			actionTilemap->Scroll(2 * dx, 2 * dy);
		}
	});
	sca->Start(sc, Time::getTime());

	Configurator mapConfig;
	mapConfig.Configure(Resources::Get().GetResource(mapName));

	AggregateProperty* tilemaps =
		(AggregateProperty*)mapConfig.GetConfigurations()
			->GetProperties()
			.find("tilemap")
			->second;

	AggregateProperty* actionMap =
		(AggregateProperty*)tilemaps->GetProperties().find("action")->second;

	TilemapMaker actionMaker;
	actionMaker.Configure(*actionMap);
	actionTilemap = actionMaker.MakeTilemap();
	actionTilemap->SetView(
		{0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()});

	AggregateProperty* bgMap = (AggregateProperty*)tilemaps->GetProperties()
								   .find("background")
								   ->second;

	TilemapMaker bgMaker;
	bgMaker.Configure(*bgMap);
	bgTilemap = bgMaker.MakeTilemap();

	bg = new CircularBackground(bgTilemap->GetTilemap()->Copy());
	bg->SetView({0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()});
	System::Destroy(bgTilemap);
}

void Unit3::Clear() {
	System::Destroy(window);
	System::Destroy(renderer);

	System::Destroy(actionTilemap);
	System::Destroy(displayBuffer);
	System::Destroy(bg);
}
