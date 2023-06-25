#pragma once

#include "game_object.hpp"

#include "sprite_rendering_component.hpp"
#include "rigidbody_component.hpp"
#include "wandering_AI_component.hpp"
#include "animation_component.hpp"

#include "global_assets.hpp"

#include <vector>

class Zombie : public GameObject {
private:
	bool moving;

public:
	Zombie(Scene* scene);
};


