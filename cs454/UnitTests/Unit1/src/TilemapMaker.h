#pragma once

#include <GameEngine/Graphics/Tilemap.h>
#include <GameEngine/JSON/AggregateProperty.h>
#include <GameEngine/JSON/ArrayProperty.h>
#include <GameEngine/JSON/Configurable.h>
#include <GameEngine/JSON/NumericProperty.h>
#include <GameEngine/JSON/Property.h>
#include <GameEngine/JSON/StringProperty.h>
#include <cassert>
#include <string>
#include <GameEngine/Resources.h>

class TilemapMaker : public GameEngine::Json::IConfigurable {
   private:
	int width = 0, height = 0, tilew = 0, tileh = 0;
	std::string tileset;
	std::vector<int> map;

   public:
	virtual std::string GetJsonId() const override { return "tilemap"; };
	virtual bool IsValid(
		const GameEngine::Json::Property& property) const override {
		using namespace GameEngine::Json;

		AggregateProperty* ag = nullptr;
		AggregateProperty::Properties prop;
		if (property.GetType() == Property::AGGREGATE) {
			ag = (AggregateProperty*)(&property);
			prop = ag->GetProperties();
		}

		return ag != nullptr && ag->GetId() == "tilemap";
	}
	virtual void Configure(
		const GameEngine::Json::Property& property) override {
		assert(IsValid(property));
		using namespace GameEngine::Json;

		AggregateProperty* ag = (AggregateProperty*)(&property);
		AggregateProperty::Properties prop = ag->GetProperties();

		tilew = (int)((NumericProperty*)(*prop.find("tilewidth")).second)
					->GetValue();
		tileh = (int)((NumericProperty*)(*prop.find("tileheight")).second)
					->GetValue();
		width =
			(int)((NumericProperty*)(*prop.find("width")).second)->GetValue() *
			tilew;
		height =
			(int)((NumericProperty*)(*prop.find("height")).second)->GetValue() *
			tileh;
		tileset = ((StringProperty*)(*prop.find("tileset")).second)
					  ->GetValueStripped();

		ArrayProperty* array = (ArrayProperty*)prop.find("map")->second;
		std::vector<Property*> p = array->GetProperties();
		for (size_t i = 0; i < p.size(); i++) {
			map.push_back((int)((NumericProperty*)p.at(i))->GetValue() - 1);
		}
	}

	GameEngine::Graphics::Tilemap* MakeTilemap() {
		using namespace GameEngine::Graphics;
		std::string srcpath = GameEngine::Resources::Get().GetAsset(tileset);

		return new Tilemap(width, height, tilew, tileh, srcpath, map);
	}
};