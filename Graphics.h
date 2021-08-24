/**************************Graphics.h************************************
* Created: 08/18/2021
* Modified: 08/18/2021
* Developed By: Arhum Z. Nayyar
* Description: The graphics engine will update and render all drawings
************************************************************************/
#pragma once
#include <SDL.h>

struct SDL_Window;
struct SDL_Renderer;

class Graphics
{
public:
	Graphics();
	~Graphics();
	// Change the window dimensions
	void setWindowDimensions(int width, int height);
	// Draws a texture to the specified part of the screen
	void drawSurface(SDL_Texture* texture, SDL_Rect* sourceRect, SDL_Rect* destRect);
	// Render to the screen
	void renderSurface();
	// Clear the screen
	void clearAll();
	// Return the renderer
	SDL_Renderer* getRenderer() const;
private:
	SDL_Window* window;
	SDL_Renderer* renderer;

	int windowWidth = 640;
	int windowHeight = 480;
};

