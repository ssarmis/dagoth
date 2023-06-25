#pragma once

#include "render_component.hpp"
#include "transform.hpp"
#include "sprite.hpp"

#include "opengl_layer.h"
class SpriteRenderingComponent : public RenderComponent {
private:
	Texture* atlas;
	glm::vec2 tileSize;
	Transform* transform;
	SpriteTexture* texture;

public:
	SpriteRenderingComponent():RenderComponent(SPRITE_RENDERING_COMPONENT), transform(nullptr){}
	SpriteRenderingComponent(Transform* transform, SpriteTexture* texture):
		RenderComponent(SPRITE_RENDERING_COMPONENT), transform(transform), texture(texture)
		{}

	void SetAtlas(Texture* atlas);
	void SetTileSize(glm::vec2 tileSize);

	virtual void Render(RenderingObject* renderObject) override {
		RenderingObjectPushQuad(renderObject, transform->position, *texture, transform->size, atlas->width, atlas->height, tileSize, transform->mirrorX);
	}
};
