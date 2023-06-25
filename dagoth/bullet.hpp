#pragma once

#include "game_object.hpp"

#include "sprite_rendering_component.hpp"
#include "rigidbody_component.hpp"
#include "input_movement_component.hpp"
#include "animation_component.hpp"

class Bullet : public GameObject {
public:
	Bullet(const Bullet& other) = delete;
	void operator=(const Bullet& other) = delete;

	Bullet(Scene* scene, glm::vec2 position, glm::vec2 velocity);

	void OnCollision(GameObject* other);
};

