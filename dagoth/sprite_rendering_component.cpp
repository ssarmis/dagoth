#include "sprite_rendering_component.hpp"

#include "opengl_layer.h"

#if 0
void SpriteRenderingComponent::Render(RenderingObject* renderObject) {
	RenderingObjectPushQuad(renderObject, transform->position, *texture, transform->size, transform->mirrorX);
}
#endif

void SpriteRenderingComponent::SetAtlas(Texture* atlas){
	this->atlas = atlas;
}

void SpriteRenderingComponent::SetTileSize(glm::vec2 tileSize){
	this->tileSize = tileSize;
}