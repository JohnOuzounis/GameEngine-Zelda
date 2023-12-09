#pragma once

#include <GameEngine/Graphics/Image.h>
#include <GameEngine/Graphics/Rect.h>
#include <GameEngine/Graphics/Text.h>
#include <GameEngine/Graphics/UIElement.h>
#include <vector>

namespace GameEngine {
namespace Graphics {

class Button : public UIElement {
   public:
	enum State { Default, Hovered, Clicked };

   protected:
	State state;
	Text* text;
	Rect position;
	std::vector<Image*> images;
	
	Image& GetImage() const { return *images[state]; }

   public:
	Button() : state(Default), text(nullptr), position(0, 0, 0, 0) {}
	virtual ~Button();

	virtual void OnHover() = 0;
	virtual void OnClick() = 0;
	virtual void OnRelease() = 0;

	void HandleEvent();

	Text& GetText() const { return *text; }
	Rect GetPosition() const { return position; }

};
}  // namespace Graphics
}  // namespace GameEngine