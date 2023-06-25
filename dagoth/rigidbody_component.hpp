#pragma once

#include "component.hpp"
#include "tile.hpp"
#include "scene.hpp" // might move later
#include "transform.hpp"

#include <glm/vec2.hpp>
#include <functional>

using OnCollisionFunction = void (*)(GameObject* other);
using OnTriggerFunction = void (*)(GameObject* other);

enum RigidBodyComponentType {
    UNINITIALIZED,
    NORMAL,
    TRIGGER, 
};

class RigidBodyComponent : public Component {
private:
    RigidBodyComponentType type;
    bool moving;
    GameObject* object;
    Transform* transform;
    Scene* scene;

    std::function<void(GameObject* other)> onCollisionCallback;
    std::function<void(GameObject* other)> onTriggerCallback;

    bool CanPassThroughGameObjects(glm::vec2 force);
    bool CanPassThroughTiles(glm::vec2 force);

public:
    RigidBodyComponent():Component(INPUT_MOVEMENT_COMPONENT), type(UNINITIALIZED), moving(false), object(nullptr), transform(nullptr), scene(nullptr) {};
    RigidBodyComponent(GameObject* object, Transform* transform, Scene* scene):
    Component(RIGID_BODY_COMPONENT), type(NORMAL), object(object), moving(false), transform(transform), scene(scene){
        // NOTE(Sarmis) just empty functions as callback, so we don't check if we have them or not...
        onCollisionCallback = [](auto other) {};
        onTriggerCallback = [](auto other) {};
    };

    void SetCollisionCallback(std::function<void(GameObject* other)> callback);
    void SetTriggerCallback(std::function<void(GameObject* other)> callback);

    virtual void Tick(r32 delta, void* data) override;
    void MakeTrigger();
    void AddForce(glm::vec2 force);
};


