#include "player.hpp"

#include <iostream>

#include "bullet.hpp"
#include "bullet_shooting_behaviour.hpp"
#include "texture_change_behaviour.hpp"

Player::Player(Scene* scene) :GameObject(scene) {
	state.currentState = IDLE;
	state.oldState = IDLE;

	transform.position = glm::vec2(32, 32);
	texture = static_cast<SpriteTexture>(glm::vec2(0, 0));
	transform.speed = glm::vec2(100, 100);
	transform.size = glm::vec2(64, 64);
	transform.velocity = glm::vec2(0, 0);

	auto rigidBodyComponent = componentManager.AddComponent<RigidBodyComponent>(this, &transform, scene);

	rigidBodyComponent->SetTriggerCallback([](auto other){
		std::cout << "Trigger Player" << std::endl;
	});

	auto inputMovementComponent = componentManager.AddComponent<InputMovementComponent>(&transform, rigidBodyComponent);
	auto animationComponent = componentManager.AddComponent<AnimationComponent>(&texture);

	animationComponent->AddAnimation(Assets::playerIdleAnimation); // in the order from the states
	animationComponent->AddAnimation(Assets::playerWalkingAnimation); // in the order from the states

	auto spriteRenderingComponent = renderComponentManager.AddComponent<SpriteRenderingComponent>(&transform, &texture);
	
	spriteRenderingComponent->SetAtlas(&Assets::playerAtlas);
	spriteRenderingComponent->SetTileSize(glm::vec2(16, 16));

	behaviourManager.AddComponent<BulletShootingBehaviour>(this);
	behaviourManager.AddComponent<TextureChangeBehaviour>(this);
}