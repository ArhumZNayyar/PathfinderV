#include "Text.h"

void Text::drawText(Graphics & graphics, int x, int y, std::string str, int fontSize, SDL_Color color)
{
	if (!TTF_WasInit()) { // TTF isn't initialized
		std::cout << "TTF_Init failed " << TTF_GetError() << std::endl;
	}
	TTF_Font *font = TTF_OpenFont("VT323-Regular.ttf", fontSize);
	if (!font) { // Unable to load font
		printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
	SDL_Surface *surface = TTF_RenderText_Solid(font, str.c_str(), color);
	SDL_Rect destinationRectangle = { x, y, surface->w, surface->h };
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);

	graphics.drawSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
}
