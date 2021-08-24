#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <iostream>

#include "Graphics.h"

class Text
{
public:
	Text() {};
	~Text() {};

	void drawText(Graphics &graphics, int x, int y, std::string str, int fontSize, SDL_Color color);
private:
	SDL_Color fontColor = { 255, 255, 255, 255 };
	TTF_Font *font = TTF_OpenFont("VT323-Regular.ttf", 18);
};

