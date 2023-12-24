#pragma once
#include <vector>
#include <string>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Color.h>

namespace GameEngine {
class AnimationFilm {
	std::vector<Graphics::Rect> boxes;
	std::vector<Graphics::Point> offsets;
	std::vector<Graphics::Rect> colliders;
	Graphics::Image* bitmap = nullptr;
	std::string id;

   public:
	int GetTotalFrames(void) const { return (int)boxes.size(); }
	auto GetId(void) const -> const std::string& { return id; }
	Graphics::Image* GetBitmap(void) const { return bitmap; }

	const Graphics::Rect& GetFrameBox(int frameNo) const {
		assert(boxes.size() > frameNo);
		return boxes[frameNo];
	}
	const Graphics::Point& GetFrameOffset(int frameNo) const {
		assert(offsets.size() > frameNo);
		return offsets[frameNo];
	}
	const Graphics::Rect& GetCollider(int frameNo) const {
		assert(colliders.size() > frameNo);
		return colliders[frameNo];
	}

	void DisplayFrame(Graphics::Image* dest, const Graphics::Rect& displayArea, int frameNo) const {
		Graphics::Rect offset = displayArea;
		offset.x += GetFrameOffset(frameNo).x;
		offset.y += GetFrameOffset(frameNo).y;
		bitmap->BlitScaled(GetFrameBox(frameNo), *dest, offset);
	}

	void SetBitmap(Graphics::Image* b) {
		assert(!bitmap);
		bitmap = b;
	}

	void SetMask(const Graphics::Color& mask) {
		bitmap->SetColorKey(mask, true);
	}
	void Append(const Graphics::Rect& r, const Graphics::Point& offset, const Graphics::Rect& c) {
		boxes.push_back(r);
		offsets.push_back(offset);
		colliders.push_back(c);
	}

	AnimationFilm(const std::string& _id) : id(_id) {}
	AnimationFilm(Graphics::Image* b, const std::vector<Graphics::Rect>&boxes, const std::string& id);
};

}