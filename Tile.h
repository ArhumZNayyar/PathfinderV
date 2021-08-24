/**************************Tile.h************************************
* Created: 08/19/2021
* Modified: 08/23/2021
* Developed By: Arhum Z. Nayyar
* Description: Handles Tiles & Nodes
********************************************************************/
#pragma once
#include "Global.h"
#include <SDL.h>
#include "Graphics.h"
#include <vector>

class Tile
{
public:
	struct Node {
		SDL_Rect tile;
		SDL_Color tileColor = { 255,255,255,255 };
		SDL_Color finalKnownTileColor = { 166, 44, 232, 255 };

		void createNode(int row, int column);
		// Set the color to the new color using the SDL_Color struct
		void alterColor(SDL_Color color) { this->tileColor = color; }
		void draw(Graphics &graphics);

		/* Node Info */
		int rowPosition;
		int columnPosition;

		float movementCost; // The cost to move from the starting point to a given node, and that path that follows.
		float heuristicCost; // Estimated guess to move from the given node to the destination.
		float finalCost; // Sum of both costs. Used to determine which node to pick to travese (shortest path).

		bool known; // Check if the node is already known (has been traversed to).
		bool obstructed; // Check if there is an obstruction at this node.
		bool colorTransitions;

		Node* parentNode;
		std::vector<Node*> adjacentNodes;

		Node() {
			// Cost is unknown at initiation. Thus the usage of INFINITY.
			movementCost = INFINITY; 
			heuristicCost = INFINITY;
			finalCost = INFINITY;
			parentNode = nullptr;
			known = false;
			obstructed = false;
		}
	};
};

