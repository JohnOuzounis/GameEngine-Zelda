#include <GameEngine/Animation/AnimationFilmHolder.h>
#include <cassert>

using namespace GameEngine;
AnimationFilmHolder AnimationFilmHolder::holder;

void GameEngine::AnimationFilmHolder::Load(const std::string& filepath,
										   const AnimationFilmParser& parser) {
	std::list<AnimationFilm*> output;
	auto result = parser(output, filepath);
	assert(result);

	for (auto entry : output) {
		assert(!GetFilm(entry->GetId()));
		films[entry->GetId()] = entry;
	}
}

void GameEngine::AnimationFilmHolder::CleanUp(void) {
	for (auto& i : films)
		delete (i.second);
	films.clear();
}

auto GameEngine::AnimationFilmHolder::GetFilm(const std::string& id)
	-> const AnimationFilm* const {
	auto i = films.find(id);
	return i != films.end() ? i->second : nullptr;
}
