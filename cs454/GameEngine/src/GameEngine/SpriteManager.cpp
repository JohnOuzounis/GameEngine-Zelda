#include <GameEngine/SpriteManager.h>
using namespace GameEngine;
using namespace GameEngine::Graphics;

SpriteManager SpriteManager::singleton;

void GameEngine::SpriteManager::Add(Graphics::Sprite* s) {
	auto it = dpyList.begin();
	while (it != dpyList.end() && (*it)->GetZorder() < s->GetZorder()) {
		++it;
	}

	dpyList.insert(it, s);
	types[s->GetTypeId()].push_back(s);
}

void GameEngine::SpriteManager::Remove(Graphics::Sprite* s) {
	dpyList.remove(s);

	auto typeListIt = types.find(s->GetTypeId());
	if (typeListIt != types.end()) {
		typeListIt->second.remove(s);

		if (typeListIt->second.empty()) {
			types.erase(typeListIt);
		}
	}
}
