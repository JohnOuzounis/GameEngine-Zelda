#include "Unit1.h"
#include "Scene1.h"

void Unit1::Init() {
	window = new Window("Unit1", 50, 50, 640, 480, Window::Flags::Shown);
	renderer = new Renderer(*window, -1, Renderer::Accelerated);

	Scene1* scene = new Scene1(window, renderer);
	SceneManager::GetSceneManager().Add(scene);
	SceneManager::GetSceneManager().LoadScene(0);

	this->game.SetFinished([&]() { return !window->IsOpen(); });
	this->game.SetInput(std::bind(&Unit1::Input, this));
	this->game.SetRender(std::bind(&Unit1::Render, this));
}

void Unit1::Input() {
	SceneManager::GetSceneManager().GetCurrentScene()->Input();
}

void Unit1::Render() {
	SceneManager::GetSceneManager().GetCurrentScene()->Render();
}
