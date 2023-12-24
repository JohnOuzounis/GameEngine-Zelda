#pragma once
#include <GameEngine/Math.h>
#include <GameEngine/Scene.h>
#include <assert.h>
#include <vector>
#include <stdexcept>

namespace GameEngine {
class SceneChangeException : public std::exception {
   public:
	const char* what() const noexcept override {
		return "Scene changed\n";
	}
};

class SceneManager final {
   private:
	typedef std::vector<Scene*> Scenes;

	int currentScene;
	Scenes scenes;

   private:
	SceneManager() : currentScene(0) {}
	~SceneManager() {}

	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

   public:
	static SceneManager& GetSceneManager() {
		static SceneManager manager;
		return manager;
	}

	int GetCurrentSceneIndex() const { return currentScene; }
	Scene* GetCurrentScene() const { return scenes[currentScene]; }
	Scene* GetScene(int index) {
		index = Math::Clamp(index, 0, (int)scenes.size() - 1);
		return scenes[index];
	}

	void Add(Scene* scene) {
		assert(scene);
		scenes.push_back(scene);
	}

	/// <summary>
	/// index is clipped between 0 and max number of scenes
	/// </summary>
	void LoadScene(int index) {
		index = Math::Clamp(index, 0, (int)scenes.size() - 1);
		scenes[currentScene]->Save();
		scenes[currentScene]->CleanUp();
		scenes[index]->Load();
		currentScene = index;
		throw SceneChangeException();
	}

	void LoadNextScene() { LoadScene(currentScene + 1); }

	void LoadPreviousScene() { LoadScene(currentScene - 1); }

	void CleanUp() {
		for (auto scene : scenes) {
			scene->CleanUp();
			delete scene;
			scene = nullptr;
		}
		scenes.clear();
	}
};
}  // namespace GameEngine