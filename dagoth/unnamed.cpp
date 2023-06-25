#define _CRT_SECURE_NO_WARNINGS

#define NOMINMAX
#define GLEW_STATIC
#include "GL/glew.h"

#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "renderer.h"
#include "opengl_layer.h"
#include "vertex.h"

#include "tile_container.hpp"
#include "tile_texturer.hpp"
#include "tile_render_object_generator.hpp"

#include "general.h"

#include <iostream>

#include "noise.hpp"
#include "input.hpp"

#include "player.hpp"
#include "zombie.hpp"

#include "global_assets.hpp"
#include "global_settings.hpp"
#include "scene.hpp"
#include "window.hpp"


int main(){
    Window window(Settings::width, Settings::height, "Test run");

    TileToTexture::TileToTextureInitialize();

    Scene scene = { "DefaultScene" };

    scene.container.tilesAmount = 64 * 64;
    scene.container.width = 64;
    scene.container.height = 64;

    scene.container.tiles = new Tile[scene.container.tilesAmount];
    scene.AllocateGameObjectChunks();

    for (int y = 0; y < 64; ++y) {
        for (int x = 0; x < 64; ++x) {
            scene.container.tiles[x + y * 64].id = std::rand() % 5;
            scene.container.tiles[x + y * 64].properties.solid = false;
        }
    }

    Assets::AssetsInitialize();

    auto playerGameObject = scene.AddGameObject<Player>(&scene);
    auto zombieGameObject = scene.AddGameObject<Zombie>(&scene);

    scene.camera.FollowGameObject(playerGameObject);

    window.MainLoop(&scene);
    return 0;
}
