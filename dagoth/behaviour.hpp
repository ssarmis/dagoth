#pragma once

#include "general.h"
#include "component_manager_interface.hpp"

class GameObject;

class Behaviour {
public:
	GameObject* parent;

	Behaviour():parent(nullptr) {}
	explicit Behaviour(GameObject* parent) :parent(parent) {}

	virtual void Tick(r32 delta, void* data) = 0;
};

class BehaviourManager : public ComponentManagerInterface<Behaviour> {
public:
	BehaviourManager() {};

	BehaviourManager(BehaviourManager& other) = delete;
	void operator=(BehaviourManager& other) = delete;

	auto begin() { return components.begin(); }
	auto end() { return components.end(); }
};
