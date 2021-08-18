#pragma once

struct SDL_Window;
struct SDL_Renderer;

class Graphics
{
public:
	Graphics();
	~Graphics();
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
};

