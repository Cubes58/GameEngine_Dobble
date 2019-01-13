#include "EntitySystem.h"

#include "EntityManager.h"

ComponentContainer &EntitySystem::GetComponentContainer() {
	return EntityManager::Instance().m_ComponentContainer;
}