#pragma once
#include <GameEngine/Animation/AnimationFilm.h>
#include <GameEngine/Clipper.h>
#include <GameEngine/Collider.h>
#include <GameEngine/Graphics/Image.h>
#include <GameEngine/GravityHandler.h>
#include <GameEngine/MotionQuantizer.h>
#include <functional>
#include <GameEngine/LatelyDestroyable.h>

namespace GameEngine {
namespace Graphics {

class Sprite : public LatelyDestroyable {
   public:
	using Mover = std::function<void(const Graphics::Rect&, int* dx, int* dy)>;

   protected:
	int frameNo = 0;
	Graphics::Rect frameBox;  // inside the film
	int x = 0, y = 0;
	bool isVisible = false;
	bool directMotion = false;
	GravityHandler gravity;

	AnimationFilm* currFilm = nullptr;
	Collider2D* boundingArea = nullptr;

	unsigned zorder = 0;
	std::string typeId, stateId;
	Mover mover;
	MotionQuantizer quantizer;

   public:
	GravityHandler& GetGravityHandler(void) { return gravity; }
	MotionQuantizer& GetMotionQuantizer() { return quantizer; }

	Sprite& SetHasDirectMotion(bool v) {
		directMotion = v;
		return *this;
	}
	bool GetHasDirectMotion(void) const { return directMotion; }

	void SetMover(const Mover& f) {
		quantizer.SetMover(mover = f);
	}
	const Graphics::Rect GetBox(void) const {
		return {x + currFilm->GetFrameOffset(frameNo).x,
				y + currFilm->GetFrameOffset(frameNo).y, frameBox.width,
				frameBox.height};
	}

	Sprite& Move(int dx, int dy) {
		if (directMotion)  // apply unconditionally offsets!
			MoveDirect(dx,dy);
		else {
			quantizer.Move(GetBox(), &dx, &dy);
			gravity.Check(GetBox());
		}
		return *this;
	}
	Sprite& MoveDirect(int dx, int dy) {
		x += dx, y += dy;
		return *this;
	}

	void SetPos(int _x, int _y) {
		x = _x;
		y = _y;
	}
	void SetZorder(unsigned z) { zorder = z; }
	unsigned GetZorder(void) const { return zorder; }

	int GetFrame(void) const { return frameNo; }
	void SetFrame(int i) {
		//if (i != frameNo) {
			assert(i < currFilm->GetTotalFrames());
			frameBox = currFilm->GetFrameBox(frameNo = i);
		//}
	}
	AnimationFilm* GetFilm() const { return currFilm; }
	void SetFilm(AnimationFilm* film) { currFilm = film; }

	void SetCollider(const Collider2D& area) {
		assert(!boundingArea);
		boundingArea = area.Clone();
	}
	void SetCollider(Collider2D* area) {
		assert(!boundingArea);
		boundingArea = area;
	}

	auto GetCollider(void) const -> const Collider2D* { return boundingArea; }
	auto GetTypeId(void) -> const std::string& { return typeId; }
	void SetVisibility(bool v) { isVisible = v; }
	bool IsVisible(void) const { return isVisible; }

	bool CollisionCheck(const Sprite* s) const;
	void Display(Graphics::Image* dest,
				 const Graphics::Rect& dpyArea,
				 const Clipper& clipper) const;

	Sprite(int _x,
		   int _y,
		   AnimationFilm* film,
		   const std::string& _typeId = "");
	virtual ~Sprite();
};
}  // namespace Graphics
}  // namespace GameEngine