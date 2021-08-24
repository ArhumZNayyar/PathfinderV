/**************************PathfinderV.cpp**************************
* Created: 08/18/2021
* Modified: 08/22/2021
* Developed By: Arhum Z. Nayyar
* Description: Central hub of the program. Handles all logic & data
********************************************************************/
#include "PathfinderV.h"

namespace {
	/* Application Data */
	std::chrono::time_point<std::chrono::system_clock> start, end;

	SDL_Color gridBG = { 22, 22, 22, 255 }; // Light black
	SDL_bool activeMouse = SDL_FALSE;
	SDL_bool mouseHover = SDL_FALSE;

	bool startActive = true;
	bool endActive = false;
	bool wallActive = false;
	bool removeActive = false;
	bool showNotice = false;
	bool idle = true;

	std::string algoName = "NOT SET";
	double algoTime = 0;

	int noticeTimer = 4000;
	int textDuration = 0;
}

PathfinderV::PathfinderV()
{
	std::cout << "PathfinderV: Intializing..." << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
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
			else if (removeActive && !startActive && !endActive && !wallActive) {
				map->removeWall(mouseY / globals::tileExtent, mouseX / globals::tileExtent);
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
					else if (!wallActive && removeActive)
						map->removeWall(mouseY / globals::tileExtent, mouseX / globals::tileExtent);
				}
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_Q)) {
			// Set either the start or end point
			removeActive = false;
			if (!wallActive) {
				endActive = false;
				startActive = !startActive;
			}
			else {
				textDuration = 0;
				showNotice = true;
			}
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_W)) {
			removeActive = false;
			if (!wallActive) {
				startActive = false;
				endActive = !endActive;
			}
			else {
				textDuration = 0;
				showNotice = true;
			}
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_E)) {
			// Create walls
			wallActive = !wallActive;
			startActive = false;
			endActive = false;
			removeActive = false;
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_S)) {
			// Solve the map
			if (this->isSolvable()) {
				this->resolveMap = true;
			}
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_1)) {
			// Set to A-Star algorithm
			delete algo;
			algo = new AStar(*map);
			algoName = "A* Search";
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_R)) {
			wallActive = false;
			startActive = false;
			endActive = false;
			removeActive = !removeActive;
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_C)) {
			// Clear the grid
			map->clearMap();
		}

		else if (input.wasKeyPressed(SDL_SCANCODE_M)) {
			map->clearMap();
			map->createMaze(graphics, 0, 0, globals::maxRows, globals::maxColumns);
		}

		const int currentTimeMS = SDL_GetTicks();
		int elapsedTimeMS = currentTimeMS - lastUpdateTimeMS;
		this->update(graphics, std::min(elapsedTimeMS, globals::maxFrameTime));
		lastUpdateTimeMS = currentTimeMS;

		if (showNotice) {
			textDuration += std::min(elapsedTimeMS, globals::maxFrameTime);
			if (textDuration >= noticeTimer)
				showNotice = false;
		}

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
	// Draw data
	text->drawText(graphics, 625, 5, "----------Info----------------", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 25, ">>Algorithm: " + algoName, 20, SDL_Color{ 255, 255, 255, 255 });
	if (startActive)
		text->drawText(graphics, 625, 50, ">>Selection: Start Point", 20, SDL_Color{ 255, 255, 255, 255 });
	else if (endActive)
		text->drawText(graphics, 625, 50, ">>Selection: Destination", 20, SDL_Color{ 255, 255, 255, 255 });
	else if (wallActive)
		text->drawText(graphics, 625, 50, ">>Selection: Walls", 20, SDL_Color{ 255, 255, 255, 255 });
	else if (removeActive)
		text->drawText(graphics, 625, 50, ">>Selection: Remove Walls", 20, SDL_Color{ 255, 255, 255, 255 });
	else
		text->drawText(graphics, 625, 50, ">>Selection: NONE", 20, SDL_Color{ 255, 255, 255, 255 });
	if (showNotice) {
		text->drawText(graphics, 125, 220, "Disable Wall creation first!", 40, SDL_Color{ 255, 255, 255, 255 });
	}
	if (map->getStartingPoint() != nullptr) {
		text->drawText(graphics, 625, 75, ">>Start Point: " + std::to_string(map->getStartingPoint()->rowPosition)
			+ "," + std::to_string(map->getStartingPoint()->columnPosition), 20, SDL_Color{ 255, 255, 255, 255 });
	}
	else {
		text->drawText(graphics, 625, 75, ">>Start Point: ", 20, SDL_Color{ 255, 255, 255, 255 });
	}
	if (map->getDestination() != nullptr) {
		text->drawText(graphics, 625, 100, ">>Destination: " + std::to_string(map->getDestination()->rowPosition)
			+ "," + std::to_string(map->getDestination()->columnPosition), 20, SDL_Color{ 255, 255, 255, 255 });
	}
	else {
		text->drawText(graphics, 625, 100, ">>Destination: ", 20, SDL_Color{ 255, 255, 255, 255 });
	}
	text->drawText(graphics, 625, 125, "----------Result--------------", 20, SDL_Color{ 255,255,255,255 });
	if (idle)
		text->drawText(graphics, 625, 150, ">>Status: Idle", 20, SDL_Color{ 255,255,255,255 });
	std::string sTime = ">>Search Time: " + std::to_string(algoTime);
	sTime = sTime.substr(0, sTime.find(".") + 2 + 1);
	text->drawText(graphics, 625, 175, sTime + " seconds", 20, SDL_Color{ 255, 255, 255, 255 });
	// Draw controls
	text->drawText(graphics, 625, 205, "----------Controls------------", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 230, "[Q] = Set starting point", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 255, "[W] = Set destination point", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 280, "[E] = Create walls", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 305, "[R] = Remove walls", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 330, "[S] = Solve map", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 355, "[C] = Clear map", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 380, "[M] = Generate Maze", 20, SDL_Color{ 255, 255, 255, 255 });
	text->drawText(graphics, 625, 405, "[1] = A* Search Algorithm", 20, SDL_Color{ 255, 255, 255, 255 });
	// Build Version & Developer
	text->drawText(graphics, 625, 580, "Developed by Arhum Z. Nayyar", 20, SDL_Color{ 255,255,255,255 });
	text->drawText(graphics, 625, 600, "BUILD VER: 2.0.1.4", 20, SDL_Color{ 255, 255, 255, 255 });

	// Present to screen
	graphics.renderSurface();
}

void PathfinderV::update(Graphics &graphics, float elapsedTime)
{
	if (map->getStartingPoint() && map->getDestination() && algo) {
		if (this->resolveMap && idle) {
			idle = false;
		}
		else if (this->resolveMap && !idle) {
			text->drawText(graphics, 625, 150, ">>Status: Searching...", 20, SDL_Color{ 255,255,255,255 });
			start = std::chrono::system_clock::now();
			map->resetMap();
			if (algo->findSolution(graphics)) {
				algo->createPath(graphics);
			}
			end = std::chrono::system_clock::now();
			std::chrono::duration<double> completionTime = end - start;
			algoTime = completionTime.count();
			std::cout << "Time elapsed: " << completionTime.count() << " ms" << std::endl;
			this->resolveMap = false;
			idle = true;
		}
	}
	else
		idle = true;
}