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
	friend EntitySystem;

	std::hash<std::string> m_StringHasher;
	std::set<EntityID> m_Entities;
	ComponentContainer m_ComponentContainer;

	std::vector<std::shared_ptr<EntitySystem>> m_Systems;
	//std::shared_ptr<RenderSystem> m_RenderSystem;

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

		// If server - load all the data from files.
		// If client - load a bunch of data, but also get some from the server.

		alreadyInstantiated = true;
	}

	void InvokeSystems() {
		for (auto &entity : m_Entities) {
			for (auto &system : m_Systems) {
				system->Process(entity);
			}
		}
	}

	EntityID CreateEntity(const EntityID &p_EntityID) {
		m_Entities.emplace(p_EntityID);

		return p_EntityID;
	}
	EntityID CreateEntity(const std::string &p_EntityName) {
		return CreateEntity(m_StringHasher(p_EntityName));
	}

	template <typename ComponentType>
	void AddComponentToEntity(const EntityID &p_EntityID, ComponentType p_Component) {
		auto entity = m_Entities.find(p_EntityID);
		if (entity != m_Entities.end())
			m_ComponentContainer.AddComponent(p_EntityID, p_Component);
	}
	template <typename ComponentType>
	void AddComponentToEntity(const std::string &p_EntityName, ComponentType p_Component) {
		AddComponentToEntity(m_StringHasher(p_EntityName), p_Component);
	}

	template <typename ComponentType>
	void RemoveComponent(const EntityID &p_EntityID) {
		m_ComponentContainer.DeleteComponent<ComponentType>(p_EntityID);
	}

	void RemoveEntity(const EntityID &p_EntityID) {
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
	std::weak_ptr<ComponentType> GetComponent(const EntityID &p_Entity) {
		return m_ComponentContainer.GetComponent<ComponentType>(p_Entity);
	}
	template <typename ComponentType>
	std::weak_ptr<ComponentType> GetComponent(const std::string &p_Entity) {
		return GetComponent<ComponentType>(m_StringHasher(p_Entity));
	}

	void AddSystem(std::shared_ptr<EntitySystem> p_EntitySystem) {
		m_Systems.push_back(p_EntitySystem);
	}

	void Clear() {
		m_Entities.clear();
		m_Systems.clear();
	}
};