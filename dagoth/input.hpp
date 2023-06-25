#pragma once

#include "SDL2/SDL.h"

class Input {
private:
	static bool keysPressed[255];
	static bool keysReleased[255];

public:
	static void Tick(SDL_Event* event);
	static void ResetKeys();
	static bool KeyPressed(SDL_Keycode code);
	static bool KeyReleased(SDL_Keycode code);
};