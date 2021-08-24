#pragma once

namespace globals {
	/* Application Info */
	const int FPS = 60;
	const int maxFrameTime = 1000 / FPS;
	const int screenWidth = 815;
	const int screenHeight = 715;

	/* Map Info */
	// Map dimensions (Note: It is less than the screen width/height) so text can be displayed outside the map
	const int mapWidth = 620;
	const int mapHeight = 620;
	const int maxRows = 31;
	const int maxColumns = 31;
	const int tileExtent = 20;
}

