/**************************Algorithms.cpp****************************
* Created: 08/20/2021
* Modified: 08/23/2021
* Developed By: Arhum Z. Nayyar
* Description: Holds all the algorithms and their functions
********************************************************************/
#include "Algorithms.h"

Algorithms::Algorithms(Map & map) :
	map(map)
{
	// Holds the algorithm object to use
}

void Algorithms::createPath(Graphics &graphics)
{
	// Draw the path by replacing the rectangles colors
	std::stack<Tile::Node*> pathNodes;
	Tile::Node* presentNode = destination->parentNode;
	while (presentNode->parentNode != nullptr) {
		pathNodes.push(presentNode);
		presentNode = presentNode->parentNode;
	}
	while (!pathNodes.empty()) {
		presentNode = pathNodes.top();
		presentNode->alterColor(this->pathColor);
		presentNode->colorTransitions = false;
		this->map.draw(graphics);
		SDL_RenderPresent(graphics.getRenderer());
		SDL_Delay(8);
		pathNodes.pop();
	}
}

/* A* algorithm */
AStar::AStar(Map & map) :
	Algorithms{map} {}

bool AStar::findSolution(Graphics &graphics)
{
	std::vector<Tile::Node*> tileNodes;
	this->startPoint = map.getStartingPoint();
	this->destination = map.getDestination();
	this->startPoint->movementCost = 0;
	this->startPoint->heuristicCost = calculateNodeDistance(this->startPoint, this->destination);
	this->startPoint->finalCost = startPoint->heuristicCost;
	this->startPoint->known = true;
	// Once the data is populated insert into the vector
	tileNodes.push_back(startPoint);
	int timeToUpdate = 10;
	int timer = 0;
	int lastUpdateTimeMS = SDL_GetTicks();
	SDL_Event event;
	while (!tileNodes.empty()) {
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			}
		}
		// lambda function to sort the vector by its costs
		std::sort(tileNodes.begin(), tileNodes.end(), [](const Tile::Node* previousNode, const Tile::Node* nextNode) 
			{ return previousNode->finalCost > nextNode->finalCost; });
		Tile::Node* presentNode = tileNodes.back();
		tileNodes.pop_back();

		this->map.draw(graphics);
		SDL_RenderPresent(graphics.getRenderer());
		while (timer < timeToUpdate) {
			const int currentTimeMS = SDL_GetTicks();
			int elapsedTimeMS = currentTimeMS - lastUpdateTimeMS;
			elapsedTimeMS = std::min(elapsedTimeMS, globals::maxFrameTime);
			lastUpdateTimeMS = currentTimeMS;

			timer += elapsedTimeMS;
		}
		timer = 0;
		for (auto& adjacentNode : presentNode->adjacentNodes) {
			if (!adjacentNode->obstructed) {
				if (adjacentNode->known) {
					if (adjacentNode->movementCost > (presentNode->movementCost + calculateNodeDistance(presentNode, adjacentNode))) 
					{
						adjacentNode->movementCost = presentNode->movementCost + calculateNodeDistance(adjacentNode, presentNode);
						adjacentNode->finalCost = adjacentNode->movementCost + adjacentNode->heuristicCost;
						adjacentNode->parentNode = presentNode;
					}
				}
				else {
					adjacentNode->known = true;
					adjacentNode->parentNode = presentNode;
					// Destination was found
					if (adjacentNode == destination) 
					{
						return true;
					}
					adjacentNode->alterColor(this->knownTileColor);
					adjacentNode->colorTransitions = true;
					adjacentNode->movementCost = presentNode->movementCost + calculateNodeDistance(presentNode, adjacentNode);
					adjacentNode->heuristicCost = calculateNodeDistance(adjacentNode, destination);
					adjacentNode->finalCost = adjacentNode->movementCost + adjacentNode->heuristicCost;
					tileNodes.push_back(adjacentNode);
				}
			}
		}
	}
	return false;
}
