#include "bullet.hpp"

#include <iostream>
#include <functional>

#include "global_assets.hpp"

Bullet::Bullet(Scene* scene, glm::vec2 position, glm::vec2 velocity) :GameObject(scene) {
	state.currentState = IDLE;
	state.oldState = IDLE;

	transform.position = position;
	texture = static_cast<SpriteTexture>(glm::vec2(0, 0));
	transform.speed = glm::vec2(100, 100);
	transform.size = glm::vec2(16, 16);
	transform.velocity = velocity;

	auto rigidBodyComponent = componentManager.AddComponent<RigidBodyComponent>(this, &transform, scene);

	//rigidBodyComponent->SetCollisionCallback(std::bind(&Bullet::OnCollision, this, std::placeholders::_1));
	rigidBodyComponent->SetCollisionCallback([=](auto other) {
		std::cout << "Removing..." << std::endl;
		scene->RemoveGameObject(this);
	});

	auto spriteRenderingComponent = renderComponentManager.AddComponent<SpriteRenderingComponent>(&transform, &texture);
	spriteRenderingComponent->SetAtlas(&Assets::playerAtlas);
	spriteRenderingComponent->SetTileSize(glm::vec2(16, 16));
}

