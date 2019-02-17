#pragma once

#include <map>
#include <unordered_map>
#include <set>
#include <typeindex>
#include <functional>
#include <string>
#include <memory>

#include "EntitySystem.h"
#include "Component.h"

#include "Logger.h"

#define EntityManagerInstance EntityManager::Instance()

using EntityID = std::size_t;
class Window;

class EntityManager {
private:
	friend EntitySystem;
	
	std::unordered_map<std::type_index, std::map<EntityID, std::shared_ptr<Component>>> m_Components;
	std::set<EntityID> m_Entities;
	std::vector<std::shared_ptr<EntitySystem>> m_Systems;
	std::hash<std::string> m_StringHasher;

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

		alreadyInstantiated = true;
	}

	EntityID CreateEntity(const EntityID &p_EntityID) {
		m_Entities.emplace(p_EntityID);

		return p_EntityID;
	}
	EntityID CreateEntity(const std::string &p_EntityName) {
		return CreateEntity(m_StringHasher(p_EntityName));
	}

	const std::set<EntityID> *GetEntities() const {
		return &m_Entities;
	}

	template <typename ComponentType>
	void AddComponentToEntity(const EntityID &p_EntityID, std::shared_ptr<ComponentType> p_Component) {
		auto entity = m_Entities.find(p_EntityID);
		if (entity != m_Entities.end()) {
			m_Components[typeid(ComponentType)].emplace(p_EntityID, p_Component);
		}
	}
	template <typename ComponentType>
	void AddComponentToEntity(const std::string &p_EntityName, std::shared_ptr<ComponentType> p_Component) {
		AddComponentToEntity(m_StringHasher(p_EntityName), p_Component);
	}

	template <typename ComponentType>
	std::shared_ptr<ComponentType> GetComponent(const EntityID &p_Entity) {
		auto componentType = m_Components.find(typeid(ComponentType));
		if (componentType != m_Components.end()) {
			auto entityWithComponent = componentType->second.find(p_Entity);
			if (entityWithComponent != componentType->second.end()) {
				return std::dynamic_pointer_cast<ComponentType>(entityWithComponent->second);
			}
		}

		return nullptr;
	}
	template <typename ComponentType>
	std::shared_ptr<ComponentType> GetComponent(const std::string &p_Entity) {
		return GetComponent<ComponentType>(m_StringHasher(p_Entity));
	}

	template <typename ComponentType>
	void DeleteComponent(const EntityID &p_EntityID) {
		auto entity = m_Entities.find(p_EntityID);
		if (entity != m_Entities.end()) {
			m_Components[typeid(ComponentType)].erase(p_EntityID);
		}
	}
	template <typename ComponentType>
	void DeleteComponent(const std::string &p_EntityName) {
		DeleteComponent<ComponentType>(m_StringHasher(p_EntityName));
	}

	void DeleteEntity(const EntityID &p_EntityID) {
		auto entity = m_Entities.find(p_EntityID);
		if (entity != m_Entities.end()) {
			m_Entities.erase(entity);
			for (auto &componentType : m_Components) {
				auto iter = componentType.second.find(p_EntityID);
				if (iter != componentType.second.end()) {
					componentType.second.erase(p_EntityID);
				}
			}

		}
	}
	void DeleteEntity(const std::string &p_EntityName) {
		DeleteEntity(m_StringHasher(p_EntityName));
	}

	void AddSystem(std::shared_ptr<EntitySystem> p_EntitySystem) {
		m_Systems.push_back(p_EntitySystem);
	}

	void UpdateSystems(float p_DeltaTime) {
		for (auto &entitySystem : m_Systems) {
			entitySystem->Update(p_DeltaTime);
		}
	}

	void RenderSystems(Window &p_Window) {
		for (auto &entitySystem : m_Systems) {
			entitySystem->Render(p_Window);
		}
	}

	void Clear() {
		m_Entities.clear();
		m_Components.clear();
		m_Systems.clear();
	}
};