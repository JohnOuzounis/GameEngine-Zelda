#pragma once
#include <iostream>

#include <GameEngine/Input.h>
#include <GameEngine/ImageLoader.h>
#include <GameEngine/SceneManager.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

namespace GameEngine {

class System final {
   public:
	static bool WaitForSeconds(unsigned int seconds) {
		SDL_Delay(seconds * 1000);
		return true;
	}

	static bool Init() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
						 "SDL initialization failed: %s", SDL_GetError());
			return false;
		}

		if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
						 "SDL_image initialization failed: %s", IMG_GetError());
			return false;
		}

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
						 "SDL Mixer initialization error: %s", Mix_GetError());
			return false;
		}

		if (TTF_Init() == -1) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
						 "SDL_ttf could not initialize! SDL_ttf Error: %s",
						 TTF_GetError());
			return false;
		}

		Input::Init();

		return true;
	}

	static void CleanUp() {
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
		TTF_Quit();
		ImageLoader::GetImageLoader().CleanUp();
		SceneManager::GetSceneManager().CleanUp();
		Input::CleanUp();
	}

	template <typename T>
	static void Destroy(T*& object) {
		delete object;
		object = nullptr;
	}
};
}  // namespace GameEngine