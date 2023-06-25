#pragma once

#include "animation.hpp"
#include "component.hpp"
#include "state.hpp"
#include "sprite.hpp"
#include "input.hpp"
#include "transform.hpp"

class InputMovementComponent : public Component { // link components in the future?
private:
    bool moving;
    Transform* transform;
    std::shared_ptr<RigidBodyComponent> rigidBodyComponent;

public:
    InputMovementComponent():Component(INPUT_MOVEMENT_COMPONENT){};
    InputMovementComponent(Transform* transform, std::shared_ptr<RigidBodyComponent> rigidBodyComponent):
    Component(INPUT_MOVEMENT_COMPONENT), transform(transform), rigidBodyComponent(rigidBodyComponent)
    {};
    virtual void Tick(r32 delta, void* data) override;
};

void InputMovementComponent::Tick(r32 delta, void* data){
    StateData* state = (StateData*)(data);

	if (Input::KeyPressed('a')) {
        rigidBodyComponent->AddForce(glm::vec2(-1, 0) * transform->speed);
		moving = true;
		transform->mirrorX = true;
	} else if (Input::KeyPressed('d')){
        rigidBodyComponent->AddForce(glm::vec2(1, 0) * transform->speed);
		moving = true;
		transform->mirrorX = false;
	}

	if (Input::KeyPressed('w')) {
        rigidBodyComponent->AddForce(glm::vec2(0, -1) * transform->speed);
		moving = true;
	} else if (Input::KeyPressed('s')){
        rigidBodyComponent->AddForce(glm::vec2(0, 1) * transform->speed);
		moving = true;
	}

    if (moving) {
        state->currentState = WALKING;
	} else {
        state->currentState = IDLE;
    }

    moving = false;
	state->oldState = state->currentState;
}
