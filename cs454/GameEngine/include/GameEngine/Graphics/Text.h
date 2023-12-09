#pragma once

#include <GameEngine/Graphics/Color.h>
#include <GameEngine/Graphics/Image.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

namespace GameEngine {
namespace Graphics {
class Text {
   public:
	enum Font {
		OpenSans,
		OpenSans_Bold,
		OpenSans_Italic,
		OpenSans_BoldItalic,
		Roboto,
		Roboto_Bold,
		Roboto_Italic,
		Roboto_BoldItalic,
		Roboto_Thin,
		Roboto_ThinItalic,
		Pacifico
	};

   private:
	std::string text;
	Font font;
	int size;
	Color* color;
	std::vector<std::string> paths;

   public:
	Text(std::string text, Font font, int size, const Color& color);
	Text(std::string text, Font font, int size)
		: Text(text, font, size, {255, 255, 255}) {}

	~Text() {
		delete color;
		paths.clear();
	}

   public:
	void SetText(std::string text) { this->text = text; }
	void SetFont(const Font& font) { this->font = font; }
	void SetSize(int size) { this->size = size; }

	Font GetFont() const { return font; }
	int GetSize() const { return size; }
	std::string GetText() const { return text; }

	/// <summary>
	/// construct a new image, remember to free
	/// </summary>
	/// <returns></returns>
	Image* GetImage() const;
};
}  // namespace Graphics
}  // namespace GameEngine