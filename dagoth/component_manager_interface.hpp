#pragma once

#include <vector>
#include <memory>

template<typename ComponentBaseType>
class ComponentManagerInterface {
public:
	std::vector<std::shared_ptr<ComponentBaseType>> components;

	template<typename Type>
	Type* GetComponent() {
		const std::type_info& t = typeid(Type);
		for (auto& component : components) {
			auto c = component.get();
			if (t == typeid(*c)) {
				return (Type*)(c);
			}
		}
		return nullptr;
	}

	template<typename Type, typename ...Args>
	auto AddComponent(Args... arguments) {
		auto component = std::make_shared<Type>(std::forward<Args>(arguments)...);
		components.push_back(component);
		return component;
	}
};
