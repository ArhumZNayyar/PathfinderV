/**************************Map.h*************************************
* Created: 08/19/2021
* Modified: 08/23/2021
* Developed By: Arhum Z. Nayyar
* Description: Handles the drawings & data for the Map 
********************************************************************/
#pragma once
#include "Tile.h"
#include <SDL.h>
#include <random> // Seed generation
class Map {
public:
	Map();
	~Map() { delete[] nodes; }
	Tile::Node& getResidingNode(int row, int column) { return this->nodes[row + globals::maxColumns * column]; }
	inline Tile::Node* getStartingPoint() { return startPoint; };
	inline Tile::Node* getDestination() { return destination; };

	void draw(Graphics &graphics);
	void createStartPoint(int row, int column);
	void createDestination(int row, int column);
	void createWall(int row, int column);
	void removeWall(int row, int column);
	void toggleKnownTile();
	void createMaze(Graphics &graphics, int row, int column, int width, int height);
	// Clear the map except any created walls
	void resetMap();
	// Clear the entire map including any created walls
	void clearMap();
private:
	Tile::Node* nodes;
	Tile::Node* startPoint;
	Tile::Node* destination;
	SDL_Color defaultColor = { 44, 44, 44, 255 };
	SDL_Color startPointColor = { 18, 153, 32, 255 };
	SDL_Color destinationColor = { 161, 6, 6, 255 };
	SDL_Color wallColor = { 6, 104, 161, 255 };
	
	bool hasStartingPoint = false, hasDestination = false,
		showKnownTiles = true;
};

