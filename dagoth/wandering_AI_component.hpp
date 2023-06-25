#pragma once

#include "component.hpp"
#include "state.hpp"
#include "transform.hpp"
#include "rigidbody_component.hpp"

#include <memory>

class WanderingAIComponent : public Component { 
private:
    bool moving;
    Transform* transform;
    std::shared_ptr<RigidBodyComponent> rigidBodyComponent;
    bool continueMoving;
    glm::vec2 movingDirection;

public:
    WanderingAIComponent():Component(INPUT_MOVEMENT_COMPONENT), moving(false), transform(nullptr), rigidBodyComponent(nullptr) {};
    WanderingAIComponent(Transform* transform, std::shared_ptr<RigidBodyComponent> rigidBodyComponent):
    Component(INPUT_MOVEMENT_COMPONENT), moving(false), transform(transform), rigidBodyComponent(rigidBodyComponent)
    {};
    virtual void Tick(r32 delta, void* data) override;
};
