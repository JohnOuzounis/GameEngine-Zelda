#pragma once
#include <GameEngine/Graphics/Image.h>
#include <map>

namespace GameEngine {

class ImageLoader {
   private:
	ImageLoader() {}
	ImageLoader(const ImageLoader&) = delete;
	ImageLoader& operator=(const ImageLoader&) = delete;

	std::map<std::string, Graphics::Image*> images;
   public:

	static ImageLoader& GetImageLoader() {
		static ImageLoader loader;
		return loader;
	}

	Graphics::Image* Load(std::string path) {
		auto it = images.find(path);
		if (it != images.end())
			return it->second;

		Graphics::Image* image = new Graphics::Image(path);
		images[path] = image;
		return image;
	}

	void CleanUp() {
		for (auto& i : images) {
			delete i.second;
			i.second = nullptr;
		}
		images.clear();
	}
};
}  // namespace GameEngine