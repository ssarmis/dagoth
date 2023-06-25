#include "input_movement_component.hpp"

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
