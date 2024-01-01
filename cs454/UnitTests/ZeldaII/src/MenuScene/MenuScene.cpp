#include "MenuScene.h"
using namespace GameEngine;
using namespace GameEngine::Json;
using namespace GameEngine::Graphics;

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

void MenuScene::LoadFilms() {
	AnimationFilmHolder::Get().Load(
		Resources::Get().GetResource("config/app.json"),
		[](std::list<AnimationFilm*>& list, const std::string& filepath) {
			Configurator appConfig;
			appConfig.Configure(filepath);

			LoadFilm(appConfig, list, {"menuscene", "background"},
					 "bitmaps/menu.png");

			return true;
		});
}
