#pragma once
#include <GameEngine/Graphics/Sprite.h>
#include <list>
#include <unordered_map>

namespace GameEngine {
class SpriteManager final {
   public:
	using SpriteList = std::list<Graphics::Sprite*>;
	using TypeLists = std::unordered_map<std::string, SpriteList>;

   private:
	SpriteList dpyList;
	TypeLists types;
	static SpriteManager singleton;

   public:
	void Add(Graphics::Sprite* s);	// insert by ascending zorder
	void Remove(Graphics::Sprite* s);
	auto GetDisplayList(void) -> const SpriteList& { return dpyList; }
	auto GetTypeList(const std::string& typeId) -> const SpriteList& {
		return types[typeId];
	}
	static auto GetSingleton(void) -> SpriteManager& { return singleton; }
	static auto GetSingletonConst(void) -> const SpriteManager& {
		return singleton;
	}
};

}  // namespace GameEngine