/**************************PathfinderV.h*****************************
* Created: 08/18/2021
* Modified: 08/22/2021
* Developed By: Arhum Z. Nayyar
* Description: Central hub of the program. Handles all logic & data
********************************************************************/
#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include "Graphics.h"
#include "Input.h"
#include "Map.h"
#include "Algorithms.h"

class PathfinderV
{
public:
	PathfinderV();
	~PathfinderV();

	void pathFinder();
	void draw(Graphics &graphics);
	// void drawMap(Graphics &graphics, std::vector<SDL_Rect>& widthLayout);
	void update(Graphics &graphics, float elapsedTime);
private:
	Map* map;
	Algorithms* algo;
	bool resolveMap = false;
	bool isSolvable() 
	{ 
	bool ready; 
	return (ready = map->getDestination() && map->getStartingPoint()) 
		? (true) : (std::cout << "Failed to solve", false);
	}
};

