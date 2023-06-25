#pragma once 

#include "general.h"
#include "component_manager_interface.hpp"

#include <memory>
#include <vector>

enum ComponentType : u8 {
	ANIMATION_COMPONENT,
	INPUT_MOVEMENT_COMPONENT,
	RIGID_BODY_COMPONENT,
	PHYSICS_COMPONENT
};

class Component {
protected:
	ComponentType type;

public:
	Component(ComponentType type):type(type){};

	virtual void Tick(r32 delta, void* data) = 0;

	template<typename Type>
	static Type CastComponent(std::shared_ptr<Component> component) {
		return dynamic_cast<Type>(component.get());
	}
};

class ComponentManager : public ComponentManagerInterface<Component> {
public:
	ComponentManager() {};

	ComponentManager(ComponentManager& other) = delete;
	void operator=(ComponentManager& other) = delete;

	auto begin() { return components.begin(); }
	auto end() { return components.end(); }
};

