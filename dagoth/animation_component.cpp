#include "animation_component.hpp"

void AnimationComponent::Tick(r32 delta, void* data){
    StateData* state = (StateData*)(data);
	if (state->currentState < animations.size()) {
		if (state->oldState != state->currentState) {
			animations[state->currentState].currentTexture = 0;
			animations[state->currentState].timer = 0;
		}
	    animations[state->currentState].Tick(delta);
		*texture = animations[state->currentState].textures[animations[state->currentState].currentTexture];
	}
}