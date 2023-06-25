#pragma once

#include <glm/vec2.hpp>
#include "general.h"

struct Transform {
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec2 speed;
    glm::vec2 size;
    bool mirrorX = false;
    r32 scale;
};
