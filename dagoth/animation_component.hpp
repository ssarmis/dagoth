#pragma once

#include "animation.hpp"
#include "component.hpp"
#include "state.hpp"
#include "sprite.hpp"

#include <vector>

class AnimationComponent : public Component {
private:
    SpriteTexture* texture;
    std::vector<Animation> animations;

public:
    AnimationComponent():Component(ANIMATION_COMPONENT), texture(nullptr){};
    AnimationComponent(SpriteTexture* texture):Component(ANIMATION_COMPONENT), texture(texture) {};
    virtual void Tick(r32 delta, void* data) override;

    void AddAnimation(Animation animation){
        animations.push_back(animation);
    }
};
