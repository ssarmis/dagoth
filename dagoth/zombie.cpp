#include "zombie.hpp"

#include <iostream>

Zombie::Zombie(Scene* scene) : GameObject(scene) {
	state.currentState = IDLE;
	state.oldState = IDLE;


	u32 xx = std::rand() % 512 + 256;
	u32 yy = std::rand() % 512 + 256;

	transform.position = glm::vec2(xx, yy);
	texture = static_cast<SpriteTexture>(glm::vec2(0, 0));
	transform.speed = glm::vec2(100, 100);
	transform.size = glm::vec2(48, 48);
	transform.velocity = glm::vec2(0, 0);

	auto rigidBodyComponent = componentManager.AddComponent<RigidBodyComponent>(this, &transform, scene);

	rigidBodyComponent->MakeTrigger();
	rigidBodyComponent->SetTriggerCallback([](auto other) {
		//std::cout << "Trigger" << std::endl;
	});

	auto wanderingAIComponent = componentManager.AddComponent<WanderingAIComponent>(&transform, rigidBodyComponent);
	auto animationComponent = componentManager.AddComponent<AnimationComponent>(&texture);

	animationComponent->AddAnimation(Assets::zombieIdleAnimation); // in the order from the states
	animationComponent->AddAnimation(Assets::zombieWalkingAnimation); // in the order from the states

	auto spriteRenderingComponent = renderComponentManager.AddComponent<SpriteRenderingComponent>(&transform, &texture);
	spriteRenderingComponent->SetAtlas(&Assets::playerAtlas);
	spriteRenderingComponent->SetTileSize(glm::vec2(16, 16));
}