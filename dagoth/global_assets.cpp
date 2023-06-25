#include "global_assets.hpp"

Texture Assets::playerAtlas;

SpriteTexture* Assets::playerIdleTextures = new SpriteTexture[4];
SpriteTexture* Assets::playerWalkingTextures = new SpriteTexture[4];

Animation Assets::playerIdleAnimation;
Animation Assets::playerWalkingAnimation;

SpriteTexture* Assets::zombieIdleTextures = new SpriteTexture[1];
SpriteTexture* Assets::zombieWalkingTextures = new SpriteTexture[3];

Animation Assets::zombieIdleAnimation;
Animation Assets::zombieWalkingAnimation;