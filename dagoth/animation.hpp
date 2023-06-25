#pragma once

#include "general.h"
#include "sprite.hpp"

struct Animation {
    SpriteTexture* textures;
    r32 timer;
    int frameInterval; // TODO(Sarmis) make cerain frames last longer, not all the same amount
    int framesAmount;
    int currentTexture;

    Animation():textures(nullptr), timer(0), frameInterval(0), framesAmount(0), currentTexture(0) {}
    Animation(SpriteTexture* textures):textures(textures), timer(0), frameInterval(0), framesAmount(0), currentTexture(0) {}

    void Tick(r32 delta);
};
