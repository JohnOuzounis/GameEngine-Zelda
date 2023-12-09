#include <GameEngine/Graphics/Light2D.h>
#include <GameEngine/ImageLoader.h>

using namespace GameEngine::Graphics;

Light2D::Light2D(const Color color,
				 const Rect& position,
				 const Type& type) {
	this->color = color;
	this->position = position;
	this->type = type;

	paths.push_back("resources/light-spot.png");
	paths.push_back("resources/light-point.png");
	paths.push_back("resources/light-ambient.png");

	image = ImageLoader::GetImageLoader().Load(paths[type])->Copy();
	image->Scale(position.width, position.height);

	Image* tinted = image->Copy();
	tinted->BlitTinted({0, 0, position.width, position.height}, *image,
					   {0, 0, position.width, position.height},
					   color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
					   1);

	delete tinted;
	image->SetBlendMode(Image::Add);
}

void GameEngine::Graphics::Light2D::SetPosition(Rect& position) {
	if (position.width != this->position.width ||
		position.height != this->position.height) {
		
		this->image->Scale(position.width, position.height);
	}
	this->position = position;
}

void GameEngine::Graphics::Light2D::SetColor(Color& color) {
	if (color.r != this->color.r || color.g != this->color.g ||
		color.b != this->color.b) {

		Image* tinted = ImageLoader::GetImageLoader().Load(paths[type])->Copy();
		tinted->Scale(position.width, position.height);
		tinted->BlitTinted({0, 0, position.width, position.height}, *image,
						   {0, 0, position.width, position.height},
						   color.r / 255.0f, color.g / 255.0f, color.b / 255.0f,
						   1);
		delete tinted;
	}
	this->color = color;
}
