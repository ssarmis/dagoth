#pragma once

#include "SDL2/SDL.h"

class Scene;

struct Window {
    int width;
    int height;

    SDL_Window* internalWindow;
    SDL_GLContext glContext;
    SDL_Event event;

    bool done;

    Window(int width, int height, const char* title);
    ~Window();
    void MainLoop(Scene* currentScene);
};
