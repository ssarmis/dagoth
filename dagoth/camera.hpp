#pragma once

#include "general.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <memory>

class RenderingProgram;
class GameObject;

struct Camera {
    glm::vec2 position;
    glm::vec3 currentOffset;
    std::shared_ptr<GameObject> target;

    Camera() :target(nullptr) {};

    void FollowGameObject(std::shared_ptr<GameObject> target);
    void ApplyCameraModifiers(RenderingProgram* program, r32 delta);
};