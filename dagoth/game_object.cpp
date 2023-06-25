#include "game_object.hpp"
#include "opengl_layer.h"

void GameObject::Tick(r32 delta) {
	for (auto& component : componentManager) {
		component->Tick(delta, &state);
	}

	for (auto& behaviour: behaviourManager) {
		behaviour->Tick(delta, &state);
	}
}

void GameObject::Render(RenderingObject* renderObject) {
	for (auto& component : renderComponentManager) {
		component->Render(renderObject);
	}
}
