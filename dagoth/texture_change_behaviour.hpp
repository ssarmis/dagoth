#pragma once

#include "behaviour.hpp"

#include "input.hpp"
#include "state.hpp"

#include <iostream>

class TextureChangeBehaviour : public Behaviour {
private:
    r32 timer = 0;
    u32 currentTexture = 0;
    SpriteTexture textures[4] = {
        SpriteTexture(1, 0),
        SpriteTexture(0, 0),
        SpriteTexture(3, 0),
        SpriteTexture(4, 0),
    };
    bool doneAnimating = false;
    u32 framesAmount = 2;
    u32 frameInterval = 100;
public:
	using Behaviour::Behaviour;

	virtual void Tick(r32 delta, void* data) override {
        if(parent->state.currentState == WALKING){
            timer += delta;
            if(parent->transform.velocity.x < 0){ // going left 
                if(((u32)(timer * 1000) > frameInterval) && !doneAnimating){
                    doneAnimating = false;
                    parent->texture = textures[currentTexture];
                    ++currentTexture;
                    currentTexture %= framesAmount;
                    timer = 0;
                    if (currentTexture == 1) {
                        doneAnimating = true;
                    }
                 }
            } else if(parent->transform.velocity.x > 0){ // going right
                if (((u32)(timer * 1000) > frameInterval) && !doneAnimating) {
                    doneAnimating = false;
                    parent->texture = textures[currentTexture + 2];
                    ++currentTexture;
                    currentTexture %= framesAmount;
                    timer = 0;
                    if (currentTexture == 1) {
                        doneAnimating = true;
                    }
                }
            }
        }
	}
};