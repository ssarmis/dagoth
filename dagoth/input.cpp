#include "input.hpp"

bool Input::keysPressed[255] = {};
bool Input::keysReleased[255] = {};

void Input::Tick(SDL_Event* event) {
    switch (event->type) {
        case SDL_KEYDOWN: {
                SDL_Keycode code = event->key.keysym.sym;
                if (code >= 255 || code < 0) {
                    break;
                }

                keysPressed[code] = true;
                keysReleased[code] = false;
            }
            break;
        case SDL_KEYUP: {
                SDL_Keycode code = event->key.keysym.sym;
                if (code >= 255 || code < 0) {
                    break;
                }

                keysReleased[code] = true;
                keysPressed[code] = false;
            }
            break;
    }
}

bool Input::KeyPressed(SDL_Keycode code) {
    if(code >= 255 || code < 0){
        return false;
    }
    return Input::keysPressed[code];
}

bool Input::KeyReleased(SDL_Keycode code) {
    if (code >= 255 || code < 0) {
        return false;
    }
    return Input::keysReleased[code];
}

void Input::ResetKeys() {
    //SDL_memset(keysPressed, 0, sizeof(bool) * 255); makes it more smooth, because of the hold delay
    SDL_memset(keysReleased, 0, sizeof(bool) * 255);
}
