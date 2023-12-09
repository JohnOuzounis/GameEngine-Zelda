#include "Unit1.h"

void Unit1::Init() {
	window = new Window("Unit1", 50, 50, 640, 480, Window::Flags::Shown);
	renderer = new Renderer(*window, -1, Renderer::Accelerated);

	Configurator appConfig;
	appConfig.Configure(Resources::Get().GetResource("config/app.json"));
	std::string mapName = ((StringProperty*)appConfig.GetConfigurations()
							   ->GetProperties()
							   .find("map")
							   ->second)
							  ->GetValueStripped();
	speed = (int)((NumericProperty*)appConfig.GetConfigurations()
					  ->GetProperties()
					  .find("scrollspeed")
					  ->second)
				->GetValue();
	scaleX = (int)((NumericProperty*)appConfig.GetConfigurations()
					   ->GetProperties()
					   .find("scaleX")
					   ->second)
				 ->GetValue();
	scaleY = (int)((NumericProperty*)appConfig.GetConfigurations()
					   ->GetProperties()
					   .find("scaleY")
					   ->second)
				 ->GetValue();

	Tilelayer layer;
	Configurator mapConfig;
	mapConfig.Subscibe(&layer);
	mapConfig.Configure(Resources::Get().GetResource(mapName));
	map = layer.MakeTilemap();
	map->SetView({0, 0, window->GetWidth(), window->GetHeight()});
	Image* im = map->GetTilemap();
	im->Scale(im->GetWidth() * scaleX, im->GetHeight() * scaleY);

	this->game.SetFinished([&]() { return !window->IsOpen(); });
	this->game.SetInput(std::bind(&Unit1::Input, this));
	this->game.SetRender(std::bind(&Unit1::Render, this));
}

void Unit1::Input() {
	while (Event::GetEvent().Poll()) {
		if (Event::GetEvent().GetType() == Event::Quit)
			window->Close();

		Input::HandleEvent();

		if (Input::GetKeyDown(Event::Right))
			map->Scroll(speed, 0);
		if (Input::GetKeyDown(Event::Left))
			map->Scroll(-speed, 0);
		if (Input::GetKeyDown(Event::Down))
			map->Scroll(0, speed);
		if (Input::GetKeyDown(Event::Up))
			map->Scroll(0, -speed);
		if (Input::GetKeyDown(Event::Home))
			map->SetView({0, 0, map->GetView().width, map->GetView().height});
		if (Input::GetKeyDown(Event::End)) {
			map->SetView({map->GetTilemap()->GetWidth(),
						  map->GetTilemap()->GetHeight(), map->GetView().width,
						  map->GetView().height});
			map->Scroll(1, 1);
		}
	}
	if (Input::GetMouse(Input::MouseLeft)) {
		int x = Input::GetMouseState().mouseX;
		int y = Input::GetMouseState().mouseY;

		if (x > window->GetWidth() / 2)
			map->Scroll(speed, 0);
		if (x <= window->GetWidth() / 2)
			map->Scroll(-speed, 0);
		if (y > window->GetHeight() / 2)
			map->Scroll(0, speed);
		if (y <= window->GetHeight() / 2)
			map->Scroll(0, -speed);
	}
}

void Unit1::Render() {
	if (displayBuffer)
		System::Destroy(displayBuffer);
	displayBuffer =
		Image::Create(window->GetWidth(), window->GetHeight(), {0, 0, 0, 255});
	map->Display(*displayBuffer,
				 {0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()});

	Texture tex(renderer->GetRenderer(), *displayBuffer);

	renderer->Clear();
	renderer->Copy(
		tex, {0, 0, displayBuffer->GetWidth(), displayBuffer->GetHeight()},
		{0, 0, window->GetWidth(), window->GetHeight()});
	renderer->Render();
}
