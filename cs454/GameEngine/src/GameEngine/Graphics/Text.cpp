#include <GameEngine/Graphics/Text.h>
#include <GameEngine/Resources.h>
#include <iostream>

using namespace GameEngine::Graphics;

Image* GameEngine::Graphics::Text::GetImage() const {
	TTF_Font* font = TTF_OpenFont(this->paths[this->font].c_str(), this->size);
	if (!font) {
		std::cerr << "Font could not be loaded! SDL_ttf Error: "
				  << TTF_GetError() << std::endl;
		assert(font);
		return nullptr;
	}

	SDL_Color color;
	color.r = this->color->r;
	color.g = this->color->g;
	color.b = this->color->b;
	color.a = 255;

	SDL_Surface* surface =
		TTF_RenderText_Solid(font, this->text.c_str(), color);
	if (!surface) {
		std::cerr << "Text surface could not be created! SDL_ttf Error: "
				  << TTF_GetError() << std::endl;
		assert(surface);
		return nullptr;
	}
	return new Image(surface);
}

Text::Text(std::string text, Font font, int size, const Color& color)
	: text(text), font(font), size(size) {
	assert(size > 0);
	this->color = new Color(color.r, color.g, color.b);

	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/opensans/OpenSans-Regular.ttf"));
	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/opensans/OpenSans-Bold.ttf"));
	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/opensans/OpenSans-Italic.ttf"));
	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/opensans/OpenSans-BoldItalic.ttf"));

	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/roboto/Roboto-Regular.ttf"));
	paths.push_back(
		GameEngine::Resources::Get().GetAsset("fonts/roboto/Roboto-Bold.ttf"));
	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/roboto/Roboto-Italic.ttf"));
	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/roboto/Roboto-BoldItalic.ttf"));
	paths.push_back(
		GameEngine::Resources::Get().GetAsset("fonts/roboto/Roboto-Thin.ttf"));
	paths.push_back(GameEngine::Resources::Get().GetAsset(
		"fonts/roboto/Roboto-ThinItalic.ttf"));

	paths.push_back(
		GameEngine::Resources::Get().GetAsset("fonts/pacifico/Pacifico.ttf"));
}
