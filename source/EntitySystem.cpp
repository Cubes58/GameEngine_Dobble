#include "EntitySystem.h"

#include "EntityManager.h"

EntitySystem::EntitySystem() {
	m_Entities = &EntityManagerInstance.m_Entities;
}

std::map<EntityID, std::shared_ptr<Component>> *EntitySystem::GetComponentArray(std::type_index p_TypeIndex) {
	auto iter = EntityManagerInstance.m_Components.find(p_TypeIndex);
	if (iter != EntityManagerInstance.m_Components.end())
		return &iter->second;

	return nullptr;
}