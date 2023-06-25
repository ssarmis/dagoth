#pragma once

#include "general.h"

constexpr u8 TILE_SIZE = 8;
constexpr u8 TILE_DISPLAY_SIZE = 32;

using TileId = i8;

enum TileType : i8 { // TODO(Sarmis) to be auto-generated in the future
	TILE_INVALID = -1,
	TILE_TYPE_DIRT = 0,
	TILE_TYPE_GRASS0,
	TILE_TYPE_GRASS1,
	TILE_TYPE_GRASS2,
	TILE_TYPE_GRASS3,
#if 0
	TILE_TYPE_GRASS_TL,
	TILE_TYPE_GRASS_TM,
	TILE_TYPE_GRASS_TR,

	TILE_TYPE_GRASS_ML,
	TILE_TYPE_GRASS_MM,
	TILE_TYPE_GRASS_MR,

	TILE_TYPE_GRASS_BL,
	TILE_TYPE_GRASS_BM,
	TILE_TYPE_GRASS_BR,
#endif
	TILE_TYPE_SIZE
};

struct TileProperties {
	u8 solid : 1;
	u8 liquid : 1;
};

struct Tile {
	TileId id;
	TileProperties properties;
};