/**************************Graphics.cpp**********************************
* Created: 08/18/2021
* Modified: 08/18/2021
* Developed By: Arhum Z. Nayyar
* Description: The graphics engine will update and render all drawings
************************************************************************/
#include "Graphics.h"
#include "Global.h"
#include <SDL.h>

Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(globals::screenWidth, globals::screenHeight, 0, &this->window, &this->renderer);
	SDL_SetWindowTitle(this->window, "PathfinderV");
}

Graphics::~Graphics()
{
	// Avoid memory leaks from having a pointer to an SDL rendering memory.
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
}

void Graphics::setWindowDimensions(int width, int height)
{
	SDL_SetWindowSize(this->window, width, height);
}

void Graphics::drawSurface(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destRect)
{
	SDL_RenderCopy(this->renderer, texture, sourceRect, destRect);
}

void Graphics::renderSurface()
{
	SDL_RenderPresent(this->renderer);
}

void Graphics::clearAll()
{
	SDL_RenderClear(this->renderer);
}

SDL_Renderer * Graphics::getRenderer() const
{
	return this->renderer;
}
