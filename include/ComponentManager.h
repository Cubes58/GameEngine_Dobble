#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <typeinfo>

#include "Entity.h"

template <typename ComponentType>
class ComponentManager {
private:
	std::hash<std::string> m_StringHasher;
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

	void CreateComponent(EntityID p_EntityID) {
		m_Components.emplace(p_EntityID, std::make_shared<ComponentType>());
	}
	void CreateComponent(const std::string &p_EntityName) {
		m_Components.emplace(m_StringHasher(p_EntityName), std::make_shared<ComponentType>());
	}

	void AddComponent(EntityID p_EntityID, std::shared_ptr<ComponentType> p_Component) {
		m_Components.emplace(p_EntityID, p_Component);
	}
	void AddComponent(const std::string &p_Name, std::shared_ptr<ComponentType> p_Component) {
		AddComponent(m_StringHasher(p_Name), p_Component);
	}

	void DeleteComponent(EntityID p_EntityID) {
		auto component = m_Components.find(p_EntityID);

		if (component == m_Components.end())
			return;

		m_Components.erase(component);
	}
	void DeleteComponent(const std::string &p_EntityName) {
		DeleteComponent(m_StringHasher(p_EntityName));
	}

	std::weak_ptr<ComponentType> GetComponent(EntityID p_Entity) {
		std::weak_ptr<ComponentType> component = m_Components.find(p_Entity)->second;

		if (auto potentialComponent = component.lock())
			return potentialComponent;

		return std::shared_ptr<ComponentType>(nullptr);
	}
	std::weak_ptr<ComponentType> GetComponent(const std::string &p_EntityName) {
		return GetComponent(m_StringHasher(p_EntityName));
	}
};