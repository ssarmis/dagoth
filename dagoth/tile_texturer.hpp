#pragma once

#include "tile.hpp"

#include <glm/vec3.hpp>

using TileTexture = glm::vec3;

struct TileToTexture {
	static TileTexture tiles[TILE_TYPE_SIZE]; // TODO(Sarmis) Generate this

	static void TileToTextureInitialize() {
		tiles[TILE_TYPE_DIRT] = { 0, 0, 0 };

		tiles[TILE_TYPE_GRASS0] = { 0, 0, 0 };
		tiles[TILE_TYPE_GRASS1] = { 0, 1, 0 };
		tiles[TILE_TYPE_GRASS2] = { 1, 0, 0 };
		tiles[TILE_TYPE_GRASS3] = { 1, 0, 0 };
	}
};
