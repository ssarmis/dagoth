#pragma once

#include "behaviour.hpp"

#include "input.hpp"
#include "bullet.hpp"

#include <iostream>

class BulletShootingBehaviour : public Behaviour {
public:
	using Behaviour::Behaviour;

	void Tick(r32 delta, void* data) override {
		if (Input::KeyReleased(' ')) {
			parent->scene->AddGameObject<Bullet>(parent->scene, parent->transform.position, glm::vec2(1, 0));
		}
	}
};