#pragma once
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "Graphics.h"

class Text
{
public:
	Text() {};
	~Text() {};

	void drawText(Graphics &graphics, int x, int y, std::string str);
private:
	SDL_Color fontColor = { 255, 255, 255, 255 };
};

