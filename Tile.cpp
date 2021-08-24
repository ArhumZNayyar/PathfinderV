/**************************Tile.cpp**********************************
* Created: 08/19/2021
* Modified: 08/23/2021
* Developed By: Arhum Z. Nayyar
* Description: Handles Tiles & Nodes
********************************************************************/
#include "Tile.h"

void Tile::Node::createNode(int row, int column)
{
	this->colorTransitions = false;
	this->rowPosition = row;
	this->columnPosition = column;
	this->tile = { column * globals::tileExtent, row * globals::tileExtent, globals::tileExtent, globals::tileExtent };
}

void Tile::Node::draw(Graphics & graphics)
{
	/*
	* Slowly increment / decrement the tile color. This combined with a delay gives off a nice gradient shade effect
	*/
	if (colorTransitions) {
		if (this->tileColor.r > this->finalKnownTileColor.r) {
			this->tileColor.r--;
		}
		if (this->tileColor.g > this->finalKnownTileColor.g) {
			this->tileColor.g--;
		}
		if (this->tileColor.b < this->finalKnownTileColor.b) {
			this->tileColor.b++;
		}
	}
	SDL_SetRenderDrawColor(graphics.getRenderer(), this->tileColor.r, this->tileColor.g, this->tileColor.b, this->tileColor.a);
	SDL_RenderFillRect(graphics.getRenderer(), &tile);
}