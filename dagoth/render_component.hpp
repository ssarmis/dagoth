#pragma once

#include "general.h"
#include "component_manager_interface.hpp"
#include <memory>
#include <vector>

class RenderingObject;

enum RenderComponentType : u8 {
	SPRITE_RENDERING_COMPONENT
};

class RenderComponent {
protected:
	RenderComponentType type;

public:
	RenderComponent(RenderComponentType type):type(type){};

	virtual void Render(RenderingObject* renderObject) = 0;
};

class RenderComponentManager : public ComponentManagerInterface<RenderComponent> {
public:
	RenderComponentManager() {};

	RenderComponentManager(RenderComponentManager& other) = delete;
	void operator=(RenderComponentManager& other) = delete;

	auto begin() { return components.begin(); }
	auto end() { return components.end(); }
};

