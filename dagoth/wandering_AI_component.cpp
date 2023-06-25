#include "wandering_AI_component.hpp"

void WanderingAIComponent::Tick(r32 delta, void* data){
    StateData* state = (StateData*)(data);

	if (std::rand() % 100 == 1) {
        continueMoving = true;
        movingDirection = glm::vec2(-1, 0);
		transform->mirrorX = true;
	} else if (std::rand() % 100 == 2){
        continueMoving = true;
        movingDirection = glm::vec2(1, 0);
		transform->mirrorX = false;
	}

	if (std::rand() % 100 == 3) {
        continueMoving = true;
        movingDirection = glm::vec2(0, -1);
	} else if (std::rand() % 100 == 4){
        continueMoving = true;
        movingDirection = glm::vec2(0, 1);
	}

    if (std::rand() % 100 == 5) {
        continueMoving = !continueMoving;
    }

    if (continueMoving) {
        rigidBodyComponent->AddForce(movingDirection * transform->speed);
    }

    if (continueMoving) {
        state->currentState = WALKING;
	} else {
        state->currentState = IDLE;
    }

	state->oldState = state->currentState;
}
