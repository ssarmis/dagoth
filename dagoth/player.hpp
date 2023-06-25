#pragma once

#include "game_object.hpp"

#include "sprite_rendering_component.hpp"
#include "rigidbody_component.hpp"
#include "input_movement_component.hpp"
#include "animation_component.hpp"

#include "global_assets.hpp"

#include <vector>

class Player : public GameObject {
private:
	bool moving;

public:
	Player(const Player& other) = delete;
	void operator=(const Player& other) = delete;

	Player(Scene* scene);
};
