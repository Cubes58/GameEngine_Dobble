#pragma once

#include <typeinfo>

#include "ComponentManager.h"

#include "RenderComponent.h"
#include "PositionComponent.h"

class EntitySystem;

class ComponentContainer {
private:
	ComponentManager<RenderComponent> m_RenderComponents;
	ComponentManager<PositionComponent> m_PositionComponents;

public:
	ComponentContainer() = default;
	~ComponentContainer() = default;

	template <typename ComponentType>
	void AddComponent(const EntityID &p_EntityID, ComponentType p_Component) {
		GetComponentManager<ComponentType>().AddComponent(p_EntityID, std::make_shared<ComponentType>(p_Component));
	}

	template <typename ComponentType>
	void DeleteComponent(const EntityID &p_EntityID) {
		GetComponentManager<ComponentType>().DeleteComponent(p_EntityID);
	}

	void RemoveComponentsOfEntity(const EntityID &p_EntityID) {
		//Check every ComponentManager, and remove any components related to the entity.
		//Maybe have ComponentManager inherit from an empty base class, then have that class store a static vector of type ids. 
		//When a ComponentManager class is created add the template type to the vector.
		m_RenderComponents.DeleteComponent(p_EntityID);
		m_PositionComponents.DeleteComponent(p_EntityID);
	}

	template <typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent(const EntityID &p_EntityID) {
		return GetComponentManager<ComponentType>().GetComponent(p_EntityID);
	}

	template <typename ComponentType>
	ComponentManager<ComponentType> &GetComponentManager() {
		if constexpr (std::is_same<ComponentType, RenderComponent>::value) {
			return m_RenderComponents;
		}
		else if constexpr (std::is_same<ComponentType, PositionComponent>::value) {
			return m_PositionComponents;
		}
	}
};