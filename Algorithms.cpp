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

		// Delay without putting the thread to sleep; which would cause the window to hang.
		while (timer < timeToUpdate) {
			const int currentTimeMS = SDL_GetTicks();
			int elapsedTimeMS = currentTimeMS - lastUpdateTimeMS;
			elapsedTimeMS = std::min(elapsedTimeMS, globals::maxFrameTime);
			lastUpdateTimeMS = currentTimeMS;

			timer += elapsedTimeMS;
		}
		timer = 0; // reset timer for next iteration
		for (auto& adjacentNode : presentNode->adjacentNodes) {
			if (!adjacentNode->obstructed) {
				float cost = calculateNodeDistance(presentNode, adjacentNode) + presentNode->movementCost;
				float adjCost = calculateNodeDistance(adjacentNode, presentNode);
				if (adjacentNode->known) {
					if (adjacentNode->movementCost > cost) 
					{
						adjacentNode->movementCost = adjCost + presentNode->movementCost;
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
					adjacentNode->movementCost = cost;
					adjacentNode->heuristicCost = calculateNodeDistance(adjacentNode, destination);
					adjacentNode->finalCost = adjacentNode->movementCost + adjacentNode->heuristicCost;
					adjacentNode->colorTransitions = true;
					adjacentNode->alterColor(this->knownTileColor);
					tileNodes.push_back(adjacentNode);
				}
			}
		}
	}
	return false;
}

Dijkstra::Dijkstra(Map & map) :
	Algorithms{map} {}

bool Dijkstra::findSolution(Graphics & graphics)
{
	std::priority_queue<Tile::Node*, std::vector<Tile::Node*>, nodeCmp> tileNodes;
	this->startPoint = map.getStartingPoint();
	this->destination = map.getDestination();
	this->startPoint->movementCost = 0;
	this->startPoint->known = true;
	// Once the data is populated insert into the vector
	tileNodes.push(startPoint);
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
		// Nodes are automatically sorted upon insertion by nodeCmp struct
		Tile::Node* presentNode = tileNodes.top();
		tileNodes.pop();

		map.draw(graphics);
		SDL_RenderPresent(graphics.getRenderer());

		// Delay without putting the thread to sleep; which would cause the window to hang.
		while (timer < timeToUpdate) {
			const int currentTimeMS = SDL_GetTicks();
			int elapsedTimeMS = currentTimeMS - lastUpdateTimeMS;
			elapsedTimeMS = std::min(elapsedTimeMS, globals::maxFrameTime);
			lastUpdateTimeMS = currentTimeMS;

			timer += elapsedTimeMS;
		}
		timer = 0; // reset timer for next iteration
		for (auto& adjacentNode : presentNode->adjacentNodes) {
			if (!adjacentNode->obstructed) {
				float cost = presentNode->movementCost
					+ calculateNodeDistance(presentNode, adjacentNode);
				if (adjacentNode->known) {
					if (adjacentNode->movementCost > cost) {
						adjacentNode->movementCost = presentNode->movementCost
							+ calculateNodeDistance(adjacentNode, presentNode);
						adjacentNode->parentNode = presentNode;
					}
				}
				else {
					adjacentNode->known = true;
					adjacentNode->parentNode = presentNode;
					if (adjacentNode == destination)
						return true;
					adjacentNode->movementCost = cost;
					adjacentNode->colorTransitions = true;
					adjacentNode->alterColor(this->knownTileColor);
					tileNodes.push(adjacentNode);
				}
			}
		}

	}
	return false;
}

Column::Column(Map & map) :
	Algorithms{map} {}

bool Column::findSolution(Graphics & graphics)
{
	std::set<Tile::Node*> tileNodes;
	this->startPoint = map.getStartingPoint();
	this->destination = map.getDestination();
	this->startPoint->movementCost = 0;
	this->startPoint->known = true;
	// Once the data is populated insert into the vector
	tileNodes.insert(startPoint);
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

		// Solution
		std::set<Tile::Node*, setSort> tileNodesSort(tileNodes.begin(), tileNodes.end());
		std::set<Tile::Node*>::iterator it;

		it = std::prev(tileNodes.end());
		Tile::Node* presentNode = *it;

		it = std::prev(tileNodes.end());
		tileNodes.erase(it);

		map.draw(graphics);
		SDL_RenderPresent(graphics.getRenderer());

		// Delay without putting the thread to sleep; which would cause the window to hang.
		while (timer < timeToUpdate) {
			const int currentTimeMS = SDL_GetTicks();
			int elapsedTimeMS = currentTimeMS - lastUpdateTimeMS;
			elapsedTimeMS = std::min(elapsedTimeMS, globals::maxFrameTime);
			lastUpdateTimeMS = currentTimeMS;

			timer += elapsedTimeMS;
		}
		timer = 0; // reset timer for next iteration
		for (auto& adjacentNode : presentNode->adjacentNodes) {
			if (!adjacentNode->obstructed) {
				float cost = presentNode->movementCost
					+ calculateNodeDistance(presentNode, adjacentNode);
				if (adjacentNode->known) {
					if (adjacentNode->movementCost > cost) {
						adjacentNode->movementCost = presentNode->movementCost
							+ calculateNodeDistance(adjacentNode, presentNode);
						adjacentNode->parentNode = presentNode;
					}
				}
				else {
					adjacentNode->known = true;
					adjacentNode->parentNode = presentNode;
					if (adjacentNode == destination)
						return true;
					adjacentNode->movementCost = cost;
					adjacentNode->colorTransitions = true;
					adjacentNode->alterColor(this->knownTileColor);
					tileNodes.insert(adjacentNode);
				}
			}
		}

	}
	return false;
}
