#include "Graphics.h"
#include <SDL.h>

Graphics::Graphics()
{
	SDL_CreateWindowAndRenderer(640, 480, 0, &this->window, &this->renderer);
	SDL_SetWindowTitle(this->window, "PathfinderV");
}

Graphics::~Graphics()
{
	//Avoid memory leaks from having a pointer to an SDL rendering memory dont let it hang.
	SDL_DestroyWindow(this->window);
	SDL_DestroyRenderer(this->renderer);
}
