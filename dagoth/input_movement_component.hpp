#pragma once

#include "animation.hpp"
#include "component.hpp"
#include "state.hpp"
#include "sprite.hpp"
#include "input.hpp"
#include "transform.hpp"
#include "rigidbody_component.hpp"

#include <memory>

class InputMovementComponent : public Component { 
private:
    bool moving;
    Transform* transform;
    std::shared_ptr<RigidBodyComponent> rigidBodyComponent;

public:
    InputMovementComponent():Component(INPUT_MOVEMENT_COMPONENT), moving(false), transform(nullptr), rigidBodyComponent(nullptr) {};
    InputMovementComponent(Transform* transform, std::shared_ptr<RigidBodyComponent> rigidBodyComponent):
    Component(INPUT_MOVEMENT_COMPONENT), moving(false), transform(transform), rigidBodyComponent(rigidBodyComponent)
    {};
    virtual void Tick(r32 delta, void* data) override;
};
