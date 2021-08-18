#pragma once
#include "SDL.h"
#include "Graphics.h"
#include <algorithm>

class PathfinderV
{
public:
	PathfinderV();
	~PathfinderV();

	void pathFinder();
	void draw(Graphics &graphics);
	void update(float elapsedTime);
};

