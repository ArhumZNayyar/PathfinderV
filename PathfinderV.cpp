/**************************PathfinderV.cpp**************************
* Created: 08/18/2021
* Modified: 08/22/2021
* Developed By: Arhum Z. Nayyar
* Description: Central hub of the program. Handles all logic & data
********************************************************************/
#include "PathfinderV.h"

namespace {
	int gridSize = 30;
	int gridWidth = 28;
	int gridHeight = 28;

	int windowWidth = (gridWidth * gridSize) + 1;
	int windowHeight = (gridHeight * gridSize) + 1;

	SDL_Rect startRect = {
		startRect.x = (gridWidth - 10) / 2 * gridSize,
		startRect.y = (gridHeight - 10) / 2 * gridSize,
		startRect.w = gridSize, 
		startRect.h = gridSize
	};

	SDL_Rect endRect = {
		endRect.x = (gridWidth + 10) / 2 * gridSize,
		endRect.y = (gridHeight + 10) / 2 * gridSize,
		endRect.w = gridSize,
		endRect.h = gridSize
	};

	std::vector<SDL_Rect> wallList;

	SDL_Rect gridCursorTrans = { startRect.x, startRect.y, gridSize, gridSize };

	// Dark
	SDL_Color gridBG = { 22, 22, 22, 255 }; // Light black
	SDL_Color gridLineColor = { 44, 44, 44, 255 }; // Dark Grey
	SDL_Color startRectColor = { 255, 255, 255, 255 }; // White
	SDL_Color endRectColor = { 255, 0, 0, 255 };
	SDL_Color wallRectColor = { 0, 0, 255, 255 };

	SDL_Color gridCursorTransColor = { 44, 44, 44, 255 };

	SDL_bool activeMouse = SDL_FALSE;
	SDL_bool mouseHover = SDL_FALSE;

	bool startActive = true;
	bool endActive = false;
	bool wallActive = false;
}

PathfinderV::PathfinderV()
{
	std::cout << "PathfinderV: Intializing..." << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	map = new Map();
	this->pathFinder();
}

PathfinderV::~PathfinderV()
{
	delete map;
}

void PathfinderV::pathFinder()
{
	Graphics graphics;
	SDL_Event event;
	Input input;
	int mouseX, mouseY;
	bool mouseButtonHeld = false;
	// Get the number of miliseconds since the SDL library initialized
	int lastUpdateTimeMS = SDL_GetTicks();
	std::cout << "PathfinderV: Intialized sucessfully!" << std::endl;
	// Main loop
	while (true) {
		input.beginNewFrame();
		SDL_GetMouseState(&mouseX, &mouseY);
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				return;
			}
			else if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					// Ensures key is not being held (0 state)
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				// Key was released
				input.keyUpEvent(event);
			}
		}

		if (event.type == SDL_MOUSEBUTTONDOWN) {
			mouseButtonHeld = true;
			if (startActive && !wallActive) {
				map->createStartPoint(mouseY / globals::tileExtent, mouseX / globals::tileExtent);
			}
			else if (endActive && !wallActive) {
				map->createDestination(mouseY / globals::tileExtent, mouseX / globals::tileExtent);
			}
		}

		else if (event.type == SDL_MOUSEBUTTONUP) {
			mouseButtonHeld = false;
		}

		else if (event.type == SDL_MOUSEMOTION) {
			if (mouseButtonHeld) {
				// Check if mouse coordinates are within the grid
				if (mouseX < globals::mapWidth && mouseY < globals::mapHeight) {
					// Check if walling is active
					if (wallActive)
						map->createWall(mouseY / globals::tileExtent, mouseX / globals::tileExtent);
					else if (!wallActive)
						map->removeWall(mouseY / globals::tileExtent, mouseX / globals::tileExtent);
				}
			}
		}

		//if (wallActive) {
		//	bool found = false;
		//	if (wallList.empty()) {
		//		wallList.push_back(SDL_Rect{ (event.motion.x / gridSize) * gridSize,
		//			(event.motion.y / gridSize) * gridSize,
		//			gridSize,
		//			gridSize });
		//	}
		//	else {
		//		for (int i = 0; i < wallList.size(); ++i) {
		//			if (wallList[i].x == (event.motion.x / gridSize) * gridSize
		//				&& wallList[i].y == (event.motion.y / gridSize) * gridSize) {
		//				wallList.erase(wallList.begin() + i);
		//				found = true;
		//			}
		//		}
		//		if (!found) {
		//			wallList.push_back(SDL_Rect{ (event.motion.x / gridSize) * gridSize,
		//			(event.motion.y / gridSize) * gridSize,
		//			gridSize,
		//			gridSize });
		//		}
		//	}
		//}
		//else if (startActive) {
		//	startRect.x = (event.motion.x / gridSize) * gridSize;
		//	startRect.y = (event.motion.y / gridSize) * gridSize;
		//}
		//else if (endActive) {
		//	endRect.x = (event.motion.x / gridSize) * gridSize;
		//	endRect.y = (event.motion.y / gridSize) * gridSize;
		//}

		//if (input.wasKeyPressed(SDL_SCANCODE_UP) == true) {
		//	startRect.y -= gridSize;
		//}

		//else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true) {
		//	startRect.y += gridSize;
		//}
		//
		//else if (input.wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
		//	startRect.x += gridSize;
		//}

		//else if (input.wasKeyPressed(SDL_SCANCODE_LEFT) == true) {
		//	startRect.x -= gridSize;
		//}

		if (input.wasKeyPressed(SDL_SCANCODE_E)) {
			// Set either the start or end point
			startActive = !startActive;
			endActive = !endActive;
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_R)) {
			// Create walls
			wallActive = !wallActive;
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_S)) {
			// Solve the map
			if (this->isSolvable()) {
				this->resolveMap = true;
			}
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_A)) {
			// Set to A-Star algorithm
			delete algo;
			algo = new AStar(*map);
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_C)) {
			// Clear the grid
			map->clearMap();
		}

		const int currentTimeMS = SDL_GetTicks();
		int elapsedTimeMS = currentTimeMS - lastUpdateTimeMS;
		this->update(graphics, std::min(elapsedTimeMS, globals::maxFrameTime));
		lastUpdateTimeMS = currentTimeMS;
		// draw after update is finished
		this->draw(graphics);
	}
}

void PathfinderV::draw(Graphics & graphics)
{
	graphics.clearAll();
	SDL_SetRenderDrawColor(graphics.getRenderer(), gridBG.r, gridBG.g, gridBG.b, gridBG.a);
	graphics.clearAll();
	// Do any drawings
	map->draw(graphics);
	// Present to screen
	graphics.renderSurface();


	//graphics.clearAll();
	//SDL_SetRenderDrawColor(graphics.getRenderer(), gridBG.r, gridBG.g, gridBG.b, gridBG.a);
	//graphics.clearAll();
	//SDL_SetRenderDrawColor(graphics.getRenderer(), gridLineColor.r, gridLineColor.g, gridLineColor.b, gridLineColor.a);

	//for (int x = 0; x < 1 + gridWidth * gridSize; x += gridSize) {
	//	SDL_RenderDrawLine(graphics.getRenderer(), x, 0, x, windowWidth);
	//}

	//for (int y = 0; y < 1 + gridHeight * gridSize; y += gridSize) {
	//	SDL_RenderDrawLine(graphics.getRenderer(), 0, y, windowHeight, y);
	//}

	//SDL_SetRenderDrawColor(graphics.getRenderer(), startRectColor.r, startRectColor.g, startRectColor.b, startRectColor.a);
	//SDL_RenderFillRect(graphics.getRenderer(), &startRect);
	//SDL_SetRenderDrawColor(graphics.getRenderer(), endRectColor.r, endRectColor.g, endRectColor.b, endRectColor.a);
	//SDL_RenderFillRect(graphics.getRenderer(), &endRect);

	//if (!wallList.empty()) {
	//	SDL_SetRenderDrawColor(graphics.getRenderer(), wallRectColor.r, wallRectColor.g, wallRectColor.b, wallRectColor.a);
	//	for (int i = 0; i < wallList.size(); i++) {
	//		SDL_RenderFillRect(graphics.getRenderer(), &wallList[i]);
	//	}
	//}

	//graphics.renderSurface();

	//std::cout << "Cursor x/y = " << startRect.x << "," << startRect.y << std::endl;
}

void PathfinderV::update(Graphics &graphics, float elapsedTime)
{
	if (map->getStartingPoint() && map->getDestination() && algo) {
		if (this->resolveMap) {
			map->resetMap();
			if (algo->findSolution(graphics)) {
				algo->createPath(graphics);
			}
			this->resolveMap = false;
		}
	}
}