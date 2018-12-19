#pragma once

#include <unordered_map>
#include <set>
#include <any>
#include <typeindex>
#include <functional>
#include <string>
#include <memory>

#include "Entity.h"
#include "ComponentContainer.h"
#include "EntitySystem.h"

#include "Logger.h"

#define EntityManagerInstance EntityManager::Instance()

using EntityID = std::size_t;

class EntityManager {
private:
	std::hash<std::string> m_StringHasher;
	std::set<EntityID> m_Entities;
	ComponentContainer m_ComponentContainer;
	std::vector<std::shared_ptr<EntitySystem>> m_Systems;

	EntityManager() = default;
	~EntityManager() = default;
public:
	static EntityManager &Instance() {
		static EntityManager s_EntityManager;

		return s_EntityManager;
	}

	void Init() {
		static bool alreadyInstantiated = false;
		if (alreadyInstantiated)
			return;

		// If server load all the data from files.
		// If client - load a bunch of data, but also get some from the server.

		alreadyInstantiated = true;
	}

	EntityID CreateEntity(EntityID p_EntityID) {
		m_Entities.emplace(p_EntityID);

		return p_EntityID;
	}
	EntityID CreateEntity(const std::string &p_EntityName) {
		return CreateEntity(m_StringHasher(p_EntityName));
	}

	template <typename ComponentType>
	void AddComponentToEntity(EntityID p_EntityID, ComponentType p_Component) {
		auto entity = m_Entities.find(p_EntityID);
		if (entity != m_Entities.end())
			m_ComponentContainer.AddComponent(p_EntityID, p_Component);
	}
	template <typename ComponentType>
	void AddComponentToEntity(const std::string &p_EntityName, ComponentType p_Component) {
		AddComponentToEntity(m_StringHasher(p_EntityName), p_Component);
	}

	void RemoveEntity(EntityID p_EntityID) {
		auto entity = m_Entities.find(p_EntityID);

		if (entity == m_Entities.end())
			return;

		m_Entities.erase(entity);
		m_ComponentContainer.RemoveComponentsOfEntity(p_EntityID);
	}
	void RemoveEntity(const std::string &p_EntityName) {
		RemoveEntity(m_StringHasher(p_EntityName));
	}

	template <typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent(EntityID p_Entity, ComponentType p_Component) {
		return m_ComponentContainer.GetComponent(p_Entity, p_Component);
	}
	template <typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent(const std::string &p_Entity, ComponentType p_Component) {
		return GetComponent(m_StringHasher(p_Entity), p_Component);
	}

	void Clear() {
		m_Entities.clear();
		m_Systems.clear();
	}
};