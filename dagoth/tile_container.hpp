#pragma once

#include "tile.hpp"
#include "general.h"

struct TileContainer {
	u32 tilesAmount;
	u32 width;
	u32 height;
	Tile* tiles;
};