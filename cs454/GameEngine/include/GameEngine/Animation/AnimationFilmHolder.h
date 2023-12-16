#pragma once
#include <GameEngine/Animation/AnimationFilm.h>
#include <functional>
#include <list>
#include <unordered_map>

namespace GameEngine {
class AnimationFilmHolder final {
   public:
	using AnimationFilmParser =
		std::function<bool(std::list<AnimationFilm*>& list,
						   const std::string& filepath)>;

   private:
	using Films = std::unordered_map<std::string, AnimationFilm*>;
	Films films;

	static AnimationFilmHolder holder;
	AnimationFilmHolder(void) {}
	~AnimationFilmHolder() { CleanUp(); }

   public:
	static auto Get(void) -> AnimationFilmHolder& { return holder; }
	void Load(const std::string& filepath, const AnimationFilmParser& parser);
	void CleanUp(void);
	auto GetFilm(const std::string& id) -> const AnimationFilm* const;
};
}  // namespace GameEngine