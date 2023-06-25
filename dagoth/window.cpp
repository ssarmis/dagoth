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

#include "input.hpp"

#include "player.hpp"
#include "zombie.hpp"

#include "global_assets.hpp"
#include "global_settings.hpp"
#include "scene.hpp"
#include "window.hpp"


Window::Window(int width, int height, const char* title):
width(width), height(height){
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    internalWindow = SDL_CreateWindow(title,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_OPENGL);

    glContext = SDL_GL_CreateContext(internalWindow);
    done = false;

#if 1
    int status = SDL_GL_SetSwapInterval(1);
    if (status == -1) {
        std::cerr << "Could not vsync" << std::endl;
    }
#endif
    glewInit();

}

Window::~Window(){
    SDL_DestroyWindow(internalWindow);
    SDL_Quit();
}

void Window::MainLoop(Scene* currentScene){
    Scene& scene = *currentScene;

    Texture tileSet = openglCreateTextureFromFile("res/atlas.png");
    RenderingObject* tileRenderingObject = TileRenderObjectGenerator::GenerateRenderingObjectFromTileContainer(scene.container, 64, 64);
    RenderingObject entityRenderingObject = openglCreateRenderingObjectEmpty(1000 * 4 * sizeof(Vertex), 1000 * 6 * sizeof(Index));
    Index* indices = openglGenerateQuadInGridIndices(1000 * 6);
    openglBufferIndicesData(entityRenderingObject, 0, indices, 1000 * 6);

    r32 oldFrameTime = 0;
    r32 newFrameTime = 0;
    r32 frameTime = 0;
    r32 deltaTime = 0;

    RenderingProgram program = openglCreateRenderProgram();

    openglPerspectiveSetup(program, Settings::width * 0.75, Settings::height * 0.75);
    openglCameraSetup(program);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.2, 0.2, 0.2, 1);

    while (!done) {
        while (SDL_PollEvent(&event)) {
            Input::Tick(&event);
            switch (event.type) {
                case SDL_QUIT: {
                        done = !done;
                    }
                    break;
            }
        }
        oldFrameTime = frameTime;
        frameTime = SDL_GetTicks();
        deltaTime = (frameTime - oldFrameTime) / 1000;

        // ticking
        scene.TickGameObjects(deltaTime);

        // rendering
        glClear(GL_COLOR_BUFFER_BIT);

        TileRenderObjectGenerator::FillRenderingObjectVertices(scene.container, tileRenderingObject, 0, 0, 63, 63);
        tileRenderingObject->usedVertices = 0; // reset

        scene.camera.ApplyCameraModifiers(&program, deltaTime*10);

        openglBindTexture(GL_TEXTURE_2D, tileSet, 0); // TODO(Sarmis) create texture array to not bind/unbind multiple textures
        openglRenderObject(*tileRenderingObject, program, tileRenderingObject->usedIndices, GL_TRIANGLES);

        //openglBindTexture(GL_TEXTURE_2D, randomBackgroundImage, 0); // TODO(Sarmis) create texture array to not bind/unbind multiple textures
        //openglRenderObject(randomRenderingObject, program, 6, GL_TRIANGLES);

        openglMapBuffer(&entityRenderingObject);
        scene.RenderGameObjects(&entityRenderingObject);
        openglUnmapBuffer(&entityRenderingObject);


        openglBindTexture(GL_TEXTURE_2D, Assets::playerAtlas, 0);
        openglRenderObject(entityRenderingObject, program, (entityRenderingObject.usedVertices / 4) * 6, GL_TRIANGLES);

        entityRenderingObject.usedVertices = 0; // reset

        openglClearCameraTransformations(program);

        SDL_GL_SwapWindow(internalWindow);
        Input::ResetKeys();
    }
}














#if 0
#if 0
    RenderingObject randomRenderingObject = openglCreateRenderingObjectEmpty(4 * sizeof(Vertex), 6 * sizeof(Index));
    randomRenderingObject.usedIndices = 6;
    Index* indices2 = openglGenerateQuadInGridIndices(1 * 6);
    VertexData data = {};
    data.numberOfVertecies = 4;
    data.data = new Vertex[4];
    data.data[0] = Vertex(0, 0, 0, 1, 1, 1, 0, 0, 0);
    data.data[1] = Vertex(0, 512, 0, 1, 1, 1, 0, 1, 0);
    data.data[2] = Vertex(512, 0, 0, 1, 1, 1, 1, 0, 0);
    data.data[3] = Vertex(512, 512, 0, 1, 1, 1, 1, 1, 0);
    openglBufferVertexData(randomRenderingObject, 0, data);
    openglBufferIndicesData(randomRenderingObject, 0, indices2, 6);
#endif
#endif