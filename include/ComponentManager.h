#pragma once

#include <unordered_map>
#include <memory>
#include <typeinfo>

#include "Entity.h"

template <typename ComponentType>
class ComponentManager {
private:
	std::unordered_map<EntityID, std::shared_ptr<ComponentType>> m_Components;

public:
	ComponentManager<ComponentType>() = default;
	~ComponentManager<ComponentType>() = default;

	static type_info GetType() {
		return typeid(ComponentType);
	}

	static std::string GetTypeName() {
		return typeid(ComponentType).name();
	}

	void CreateComponent(const EntityID &p_EntityID) {
		m_Components.emplace(p_EntityID, std::make_shared<ComponentType>());
	}

	void AddComponent(const EntityID &p_EntityID, std::shared_ptr<ComponentType> p_Component) {
		m_Components.emplace(p_EntityID, p_Component);
	}

	void DeleteComponent(const EntityID &p_EntityID) {
		auto component = m_Components.find(p_EntityID);

		if (component == m_Components.end())
			return;

		m_Components.erase(component);
	}

	std::weak_ptr<ComponentType> GetComponent(const EntityID &p_Entity) {
		std::weak_ptr<ComponentType> component = m_Components.find(p_Entity)->second;

		if (auto potentialComponent = component.lock())
			return potentialComponent;

		return std::shared_ptr<ComponentType>(nullptr);
	}
};