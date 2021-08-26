/**************************Algorithms.h******************************
* Created: 08/20/2021
* Modified: 08/23/2021
* Developed By: Arhum Z. Nayyar
* Description: Holds all the algorithms and their functions
********************************************************************/
#pragma once
#include "Map.h"
#include <algorithm>
#include <cmath>
#include <stack>
#include <vector>
#include <set>
#include <tuple>
#include <queue>

class Algorithms
{
public:
	Algorithms(Map& map);
	void createPath(Graphics &graphics);
	virtual bool findSolution(Graphics &graphics) = 0;
protected:
	Map& map;
	Tile::Node* startPoint;
	Tile::Node* destination;
	SDL_Color knownTileColor = { 255, 140, 0, 255 };
	SDL_Color pathColor = { 237, 199, 47, 255 };

	float calculateNodeDistance(Tile::Node* previousNode, Tile::Node* nextNode) 
	{
		return std::sqrt(std::pow(previousNode->rowPosition - nextNode->rowPosition, 2) 
			+ std::pow(previousNode->columnPosition - nextNode->columnPosition, 2));
	}
};

class AStar : public Algorithms {
public:
	AStar(Map& map);
	bool findSolution(Graphics &graphics);
};

class Dijkstra : public Algorithms {
public:
	struct nodeCmp {
		bool operator () (const Tile::Node* a, const Tile::Node* b) const {
			return a->movementCost > b->movementCost;
		}
	};
	Dijkstra(Map& map);
	bool findSolution(Graphics &graphics);
};

class Column : public Algorithms {
public:
	struct setSort {
		bool operator () (const Tile::Node* leftSide, const Tile::Node* rightSide) const {
			return leftSide->movementCost > rightSide->movementCost;
		}
	};
	Column(Map& map);
	bool findSolution(Graphics &graphics);
};
