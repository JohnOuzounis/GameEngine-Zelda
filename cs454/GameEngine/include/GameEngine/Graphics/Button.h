#pragma once

#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Graphics/Text.h>
#include <GameEngine/Graphics/UIElement.h>
#include <vector>
#include <functional>

namespace GameEngine {
namespace Graphics {

class Button : public UIElement {
   public:
	enum State { Default, Hovered, Clicked };
	using OnHover = std::function<void(void)>;
	using OnClick = std::function<void(void)>;
	using OnRelease = std::function<void(void)>;
	using OnDraw = std::function<void(Image&)>;

   protected:
	State state;
	Text* text;
	Rect position;
	std::vector<Image*> images;
	
	OnHover hover;
	OnClick click;
	OnRelease release;
	OnDraw draw;

   public:
	Button() : state(Default), text(nullptr), position(0, 0, 0, 0) {}
	~Button();

	void SetOnHover(const OnHover& h) { hover = h; }
	void SetOnClick(const OnClick& c) { click = c; }
	void SetOnRelease(const OnRelease& r) { release = r; }
	void SetOnDraw(const OnDraw& d) { draw = d; }

	void HandleEvent();

	Text* GetText() const { return text; }
	void SetText(Text* t) { text = t; }

	Rect GetPosition() const { return position; }
	void SetPosition(const Rect& r) { position = r; }

	State GetState() const { return state; }
	Image& GetImage() const { return *images[state]; }

	void AddImage(Image* i) { images.push_back(i); }

	virtual void Draw(Image& target) const override {
		if (draw)
			(draw)(target);
	}
};
}  // namespace Graphics
}  // namespace GameEngine