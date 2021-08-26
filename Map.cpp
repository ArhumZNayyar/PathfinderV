/**************************Map.cpp***********************************
* Created: 08/19/2021
* Modified: 08/23/2021
* Developed By: Arhum Z. Nayyar
* Description: Handles the drawings & data for the Map
********************************************************************/
#include "Map.h"
#include "Global.h"
#include "Graphics.h"

Map::Map() {
	this->nodes = new Tile::Node[globals::maxRows * globals::maxColumns];

	// Create the nodes
	for (int row = 0; row < globals::maxRows; row++) {
		for (int column = 0; column < globals::maxColumns; column++) {
			Tile::Node& presentNode = getResidingNode(row, column);
			presentNode.createNode(row, column);
			presentNode.alterColor(this->defaultColor); //Light black
			// Find all the adjacent nodes
			if (row > 0) {
				presentNode.adjacentNodes.push_back(&getResidingNode(row - 1, column));
			}

			if (row < globals::maxRows - 1) {
				presentNode.adjacentNodes.push_back(&getResidingNode(row + 1, column));
			}

			if (column > 0) {
				presentNode.adjacentNodes.push_back(&getResidingNode(row, column - 1));
			}
				
			if (column < globals::maxColumns - 1) {
				presentNode.adjacentNodes.push_back(&getResidingNode(row, column + 1));
			}
			// All directions have been processed
		}
	}
}

void Map::draw(Graphics &graphics) {
	for (int row = 0; row < globals::maxRows; row++) {
		for (int column = 0; column < globals::maxColumns; column++) {
			getResidingNode(row, column).draw(graphics);
		}
	}
	// Draw the map rectangles (GRID)
	for (int row = 0; row <= globals::maxRows; row++) {
		SDL_SetRenderDrawColor(graphics.getRenderer(), 22, 22, 22, 255); //Light black

		SDL_RenderDrawLine(graphics.getRenderer(), 0, globals::tileExtent * row, globals::mapWidth, globals::tileExtent * row);
		SDL_RenderDrawLine(graphics.getRenderer(), globals::tileExtent * row, 0, globals::tileExtent * row, globals::mapHeight);
	}
}

void Map::createStartPoint(int rowX, int colY) {
	Tile::Node* presentNode = &getResidingNode(rowX, colY);
	// Check if a starting point was already made & it's not set to an obstruction
	if (presentNode != this->destination && !presentNode->obstructed) {
		if (!this->hasStartingPoint) {
			this->startPoint = presentNode;
			this->startPoint->parentNode = nullptr;
			this->startPoint->alterColor(this->startPointColor);
			this->startPoint->colorTransitions = false;
			this->hasStartingPoint = true;
		}
		else {
			this->startPoint->alterColor(this->defaultColor);
			this->startPoint = presentNode;
			this->startPoint->parentNode = nullptr;
			this->startPoint->alterColor(this->startPointColor);
			this->startPoint->colorTransitions = false;
			this->hasStartingPoint = true;
		}
	}
}

void Map::createDestination(int row, int column) {
	Tile::Node* presentNode = &getResidingNode(row, column);
	// Check if a destination was already made & it's not set to an obstruction
	if (presentNode != this->startPoint && !presentNode->obstructed) {
		if (!hasDestination) {
			this->destination = presentNode;
			this->destination->alterColor(this->destinationColor);
			this->destination->colorTransitions = false;
			this->hasDestination = true;
		}
		else {
			this->destination->alterColor(this->defaultColor);
			this->destination = presentNode;
			this->destination->alterColor(this->destinationColor);
			this->destination->colorTransitions = false;
			this->hasDestination = true;
		}
	}
}

void Map::createWall(int row, int column) {
	Tile::Node* presentNode = &getResidingNode(row, column);
	// Check if the wall isn't being created at the starting point or the destination
	if (presentNode != this->startPoint && presentNode != this->destination) {
		presentNode->obstructed = true;
		presentNode->known = false;
		presentNode->parentNode = nullptr;
		presentNode->colorTransitions = false;
		presentNode->alterColor(this->wallColor);
	}
}

void Map::removeWall(int row, int pos) {
	Tile::Node* presentNode = &getResidingNode(row, pos);
	if (presentNode->obstructed) {
		presentNode->obstructed = false;
		presentNode->known = false;
		presentNode->parentNode = nullptr;
		presentNode->alterColor(this->defaultColor);
	}
}

void Map::toggleKnownTile()
{
	this->showKnownTiles = !this->showKnownTiles;
	if (!this->showKnownTiles) {
		for (int row = 0; row < globals::maxRows; row++) {
			for (int column = 0; column < globals::maxColumns; column++) {
				Tile::Node& presentNode = getResidingNode(row, column);

				if (presentNode.known && !presentNode.pathTile && &presentNode != this->destination
					&& &presentNode != this->startPoint) {
					presentNode.alterColor(this->defaultColor);
					presentNode.colorTransitions = false;
				}
			}
		}
	}
	else {
		for (int row = 0; row < globals::maxRows; row++) {
			for (int column = 0; column < globals::maxColumns; column++) {
				Tile::Node& presentNode = getResidingNode(row, column);

				if (presentNode.known && !presentNode.pathTile && &presentNode != this->destination
					&& &presentNode != this->startPoint) {
					presentNode.alterColor(presentNode.finalKnownTileColor);
				}
			}
		}
	}
}

void Map::createMaze(Graphics & graphics, int row, int column, int width, int height)
{
	std::random_device rd;
	// Mersenne twister engine
	std::mt19937 mersenne(rd());
	bool verticalDivision = false;
	if (width < 2 || height < 2) { return; }
	std::uniform_real_distribution<double> dist(1.0, 100.0);
	if (dist(mersenne) < 65.00) {
		verticalDivision = true;
	}
	if (verticalDivision) {
		int createColumn, removeRow;
		std::uniform_real_distribution<double> vd(1, INT_MAX);
		createColumn = 2 * ((int)vd(mersenne) % (width / 2)) + 1 + column;
		removeRow = 2 * ((int)vd(mersenne) % (1 + height / 2)) + row;

		for (int wall = row; wall < row + height; wall++) {
			this->createWall(wall, createColumn);
			this->draw(graphics);

			SDL_RenderPresent(graphics.getRenderer());
			SDL_Delay(globals::maxFrameTime - 10);
		}
		this->removeWall(removeRow, createColumn);
		this->createMaze(graphics, row, column, createColumn - column, height);
		this->createMaze(graphics, row, createColumn + 1, width - (createColumn - column + 1), height);
	}
	else {
		int createRow, removeColumn;
		std::uniform_real_distribution<double> vd(1, INT_MAX);
		createRow = 2 * ((int)vd(mersenne) % (height / 2)) + 1 + row;
		removeColumn = 2 * ((int)vd(mersenne) % (1 + width / 2)) + column;
		for (int wall = column; wall < column + width; wall++) {
			this->createWall(createRow, wall);
			this->draw(graphics);

			SDL_RenderPresent(graphics.getRenderer());
			SDL_Delay(globals::maxFrameTime - 10);
		}
		this->removeWall(createRow, removeColumn);
		this->createMaze(graphics, row, column, width, createRow - row);
		this->createMaze(graphics, createRow + 1, column, width, height - (createRow - row + 1));
	}
}

void Map::resetMap() {
	this->startPoint->parentNode = nullptr;
	this->destination->parentNode = nullptr;

	this->startPoint->known = false;
	this->destination->known = false;

	for (int row = 0; row < globals::maxRows; row++) {
		for (int column = 0; column < globals::maxColumns; column++) {
			Tile::Node& presentNode = getResidingNode(row, column);

			// Reset all the nodes that are known
			if (!presentNode.obstructed && &presentNode != this->startPoint && &presentNode != this->destination) {
				presentNode.known = false;
				presentNode.parentNode = nullptr;
				presentNode.pathTile = false;
				presentNode.alterColor(this->defaultColor);
				presentNode.colorTransitions = false;
			}
		}
	}
}

void Map::clearMap() {
	for (int row = 0; row < globals::maxRows; row++) {
		for (int column = 0; column < globals::maxColumns; column++) {
			Tile::Node& presentNode = getResidingNode(row, column);

			presentNode.pathTile = false;
			presentNode.known = false;
			presentNode.obstructed = false;
			presentNode.alterColor(this->defaultColor);
			presentNode.colorTransitions = false;
		}
	}
	this->startPoint = nullptr;
	this->destination = nullptr;
	this->hasStartingPoint = false;
	this->hasDestination = false;
}


