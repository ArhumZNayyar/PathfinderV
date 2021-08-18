#include "PathfinderV.h"
#include "Input.h"

namespace {
	const int FPS = 50;
	//Maximum amount of time a frame can last
	const int maxFrameTime = 5 * 1000 / FPS;
}

PathfinderV::PathfinderV()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	this->pathFinder();
}

PathfinderV::~PathfinderV()
{

}

void PathfinderV::pathFinder()
{
	Graphics graphics;
	SDL_Event event;
	Input input;

	//Get the number of miliseconds since the SDL library initialized
	int lastUpdateTimeMS = SDL_GetTicks();
	while (true) {
		input.beginNewFrame();
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					//Ensures key is not being held (0 state)
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				//Key was released
				input.keyUpEvent(event);
			}
		}

		const int currentTimeMS = SDL_GetTicks();
		int elapsedTimeMS = currentTimeMS - lastUpdateTimeMS;
		this->update(std::min(elapsedTimeMS, maxFrameTime));
		lastUpdateTimeMS = currentTimeMS;
	}

	//SDL_Window *window = SDL_CreateWindow("PathfinderV", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
//SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

//SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

//SDL_RenderClear(renderer);
//SDL_RenderPresent(renderer);

//SDL_Delay(3000);

// std::cout << "Hello World!\n";
}

void PathfinderV::draw(Graphics & graphics)
{
}

void PathfinderV::update(float elapsedTime)
{
}
