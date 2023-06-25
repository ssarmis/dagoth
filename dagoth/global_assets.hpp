#pragma once

#include "animation.hpp"
#include "sprite.hpp"
#include "opengl_layer.h"

struct Assets {
	static Texture playerAtlas;

	static SpriteTexture* playerIdleTextures;
	static SpriteTexture* playerWalkingTextures;
	
	static Animation playerIdleAnimation;
	static Animation playerWalkingAnimation;

	static SpriteTexture* zombieIdleTextures;
	static SpriteTexture* zombieWalkingTextures;

	static Animation zombieIdleAnimation;
	static Animation zombieWalkingAnimation;

	static void AssetsInitialize() {
		//Assets::playerAtlas = openglCreateTextureFromFile("res/space_shooter/Graphics/spritesheets/ship.png");
		Assets::playerAtlas = openglCreateTextureFromFile("res/player.png");
		Assets::playerIdleTextures[0] = glm::vec2(0, 0);

		Assets::playerIdleAnimation = Animation(playerIdleTextures);
		Assets::playerIdleAnimation.frameInterval = 1000;
		Assets::playerIdleAnimation.framesAmount = 1;
#if 1

		Assets::playerWalkingTextures[0] = glm::vec2(0, 1);
		Assets::playerWalkingTextures[1] = glm::vec2(1, 1);
		Assets::playerWalkingTextures[2] = glm::vec2(2, 1);
		Assets::playerWalkingTextures[3] = glm::vec2(3, 1);
		

		Assets::playerWalkingAnimation = Animation(playerWalkingTextures);
		Assets::playerWalkingAnimation.frameInterval = 100;
		Assets::playerWalkingAnimation.framesAmount = 4;

		Assets::zombieIdleTextures[0] = glm::vec2(0, 6);

		Assets::zombieWalkingTextures[0] = glm::vec2(0, 7);
		Assets::zombieWalkingTextures[1] = glm::vec2(1, 7);
		Assets::zombieWalkingTextures[2] = glm::vec2(2, 7);

		Assets::zombieIdleAnimation = Animation(zombieIdleTextures);
		Assets::zombieIdleAnimation.frameInterval = 1000;
		Assets::zombieIdleAnimation.framesAmount = 1;

		Assets::zombieWalkingAnimation = Animation(zombieWalkingTextures);
		Assets::zombieWalkingAnimation.frameInterval = 100;
		Assets::zombieWalkingAnimation.framesAmount = 3;
#endif
	}
};

