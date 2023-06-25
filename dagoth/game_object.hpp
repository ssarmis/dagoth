#pragma once

#include <functional>

#include <glm/vec2.hpp>
#include "general.h"
#include "sprite.hpp"
#include "state.hpp"
#include "transform.hpp"

#include "render_component.hpp"
#include "component.hpp"
#include "behaviour.hpp"

class RenderingObject;
class Scene;

class GameObject {
public:
	class GameObject* parent;
	Transform transform;
	SpriteTexture texture;
	Scene* scene;
	StateData state;
	bool toBeRemoved = false;
	i32 currentIndex = -1;

	ComponentManager componentManager;
	RenderComponentManager renderComponentManager;
	BehaviourManager behaviourManager;

	GameObject(Scene* scene):scene(scene) {}

	GameObject(GameObject& other) = delete;
	void operator=(GameObject& other) = delete;
	
public:
	void Tick(r32 delta);
	void Render(RenderingObject* renderObject);
};

using CallbackGameObjectFunction = std::function<void(GameObject* other)>;