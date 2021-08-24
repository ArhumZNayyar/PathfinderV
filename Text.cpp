#include "Text.h"

void Text::drawText(Graphics & graphics, int x, int y, std::string str)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	TTF_Font *font = TTF_OpenFont("VT323-Regular.ttf", 18);
	SDL_Surface *surface = TTF_RenderText_Solid(font, str.c_str(), this->fontColor);
	SDL_Rect destinationRectangle = { x, y, surface->w, surface->h };
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);

	graphics.drawSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
}
