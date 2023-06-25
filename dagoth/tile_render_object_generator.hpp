#pragma once

#include "opengl_layer.h"
#include "tile_container.hpp"
#include "tile_texturer.hpp"

#include <iostream>//printing

#include "global_settings.hpp"
struct TileRenderObjectGenerator {
    static inline void PushTileToVertexBuffer(Vertex* buffer, u32 offset, u32 x, u32 y, TileTexture texture) {
        x *= TILE_DISPLAY_SIZE;
        y *= TILE_DISPLAY_SIZE;

        r32 ax = (r32)TILE_SIZE / (r32)Settings::tileSetWidth;
        r32 ay = (r32)TILE_SIZE / (r32)Settings::tileSetHeight;

        r32 u0 = texture.x * ax;
        r32 u1 = u0 + ax;
        r32 v0 = texture.y * ay;
        r32 v1 = v0 + ay;

        buffer[offset + 0] = Vertex(x, y, 0, 1, 1, 1, u0, v0, 0);
        buffer[offset + 1] = Vertex(x, y + TILE_DISPLAY_SIZE, 0, 1, 1, 1, u0, v1, 0);
        buffer[offset + 2] = Vertex(x + TILE_DISPLAY_SIZE, y, 0, 1, 1, 1, u1, v0, 0);
        buffer[offset + 3] = Vertex(x + TILE_DISPLAY_SIZE, y + TILE_DISPLAY_SIZE, 0, 1, 1, 1, u1, v1, 0);
    }

    static void FillRenderingObjectVertices(TileContainer container, RenderingObject* object, u32 xo, u32 yo, u32 width, u32 height) {
        if (xo + width >= container.width || yo + height >= container.height){
            return;
        }
        Vertex* buffer = openglMapBuffer(object);
        if (buffer) {
            u32 offset = 0;
            for (int y = yo; y < yo + height; ++y) {
                u32 yy = y * container.width;
                for (int x = xo; x < xo + width; ++x) {
                    u32 index = x + yy;
                    PushTileToVertexBuffer(buffer, offset, x, y, TileToTexture::tiles[container.tiles[index].id]);
                    offset += 4;
                }
            }
            openglUnmapBuffer(object);
        }
    }

	static RenderingObject* GenerateRenderingObjectFromTileContainer(TileContainer container, u32 width, u32 height) {
        if (width * height != container.tilesAmount) {
            std::cerr << "Width * height isn't equal to the tiles amount, returning nullptr." << std::endl;
            return nullptr;
        }

        u32 verticesAmount = 4 * container.tilesAmount;
        u32 indicesAmount = 6 * container.tilesAmount;

        Index* indicesBuffer = openglGenerateQuadInGridIndices(indicesAmount);

        RenderingObject* result = new RenderingObject();
        *result = openglCreateRenderingObject(verticesAmount * sizeof(Vertex), indicesAmount * sizeof(Index));

        openglBufferIndicesData(*result, 0, indicesBuffer, indicesAmount);

        return result;
	}
};