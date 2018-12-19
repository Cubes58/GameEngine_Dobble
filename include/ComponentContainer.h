#pragma once

#include <type_traits>

#include "ComponentManager.h"

#include "RenderComponent.h"
#include "PositionComponent.h"

/*
template<typename T>
struct foo {
  friend class bar<T>;
};

Use a factory to create a bunch of components.
*/

class ComponentContainer {
private:
	ComponentManager<RenderComponent> m_RenderComponents;
	ComponentManager<PositionComponent> m_PositionComponents;
public:
	ComponentContainer() = default;
	~ComponentContainer() = default;

	template <typename ComponentType>
	void AddComponent(EntityID p_EntityID, ComponentType p_Component) {
		GetComponentManager(p_Component).AddComponent(p_EntityID, std::make_shared<ComponentType>(p_Component));
	}

	template <typename ComponentType>
	void DeleteComponent(EntityID p_EntityID, ComponentType p_Component) {
		GetComponentManager(p_Component).DeleteComponent(p_EntityID);
	}

	void RemoveComponentsOfEntity(EntityID p_EntityID) {
		//Check every ComponentManager, and remove any components related to the entity.
		m_RenderComponents.DeleteComponent(p_EntityID);
		m_PositionComponents.DeleteComponent(p_EntityID);
	}

	template <typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent(EntityID p_EntityID, ComponentType p_Component) {
		return GetComponentManager(p_Component).GetComponent(p_EntityID);
	}

	template <typename ComponentType>
	ComponentManager<ComponentType> &GetComponentManager(ComponentType p_Component) {
		if constexpr (std::is_same<ComponentType, RenderComponent>::value) {
			return m_RenderComponents;
		}
		else if constexpr (std::is_same<ComponentType, PositionComponent>::value) {
			return m_PositionComponents;
		}
	}
};