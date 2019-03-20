/**
@file EntityManager.h
@brief Manages entites.
*/
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

#include "RenderComponent.h"
#include "TransformComponent.h"

#include "Logger.h"

#define EntityManagerInstance EntityManager::Instance()	//!< Direct access to the EntityManager singleton instance.

using EntityID = std::size_t;	//!< Using EntityID as std::size_t
class Window;

/*! \class EntityManager
	\brief Manages entites.
*/

class EntityManager {
private:
	/*! 
		* @brief The Entity System class can access all of the entity managers properties.
		* @relates EntitySystem
	*/
	friend EntitySystem;
	
	std::unordered_map<std::type_index, std::map<EntityID, std::shared_ptr<Component>>> m_Components;	//!< An unordered map of component types and maps to entities (IDs) and components. 
	std::set<EntityID> m_Entities;	//!< A set of entity IDs.
	std::vector<std::shared_ptr<EntitySystem>> m_Systems;	//!< A vector of entity systems.
	std::hash<std::string> m_StringHasher;	//!< A string hasher, used to hash strings, into integers.
	bool m_Instantiated = false;

	EntityManager() = default;	//!< Constructor.
	~EntityManager() = default;	//!< Destructor.

public:
	/*!
		\brief Gets the EntityManager instance.
		\return Returns the Entity Manager instance.
	*/
	static EntityManager &Instance() {
		static EntityManager s_EntityManager;

		return s_EntityManager;
	}

	/*!
		\brief Initializes the Entity manager instance.
		\return Nothing.
	*/
	void Init() {
		if (m_Instantiated)
			return;

#ifdef GAME_ENGINE
		m_Components.reserve(2);
		m_Components[typeid(RenderComponent)];
		m_Components[typeid(TransformComponent)];
#endif

		m_Instantiated = true;
	}

	/*!
		\brief Creates an entity.
		\param p_EntityID The entity ID, to add to the entity manager.
		\return Returns the entity's ID.
	*/
	EntityID CreateEntity(const EntityID &p_EntityID) {
		m_Entities.emplace(p_EntityID);

		return p_EntityID;
	}
	/*!
		\brief Creates an entity.
		\param p_EntityName The entity's name, to add to the entity manager.
		\return Returns the entity's ID.
	*/
	EntityID CreateEntity(const std::string &p_EntityName) {
		return CreateEntity(m_StringHasher(p_EntityName));
	}

	/*!
		\brief Returns the a pointer to a set of the entities, stored in the entity manager.
		\return Returns the a pointer to a set of the entities, stored in the entity manager.
	*/
	const std::set<EntityID> *GetEntities() const {
		return &m_Entities;
	}

	/*!
		\brief Adds a component to an entity.
		\tparam ComponentType the type of component to add.
		\param p_EntityID The ID of the entity to add the component to.
		\param p_Component A shared pointer to the component that's being added to the entity.
		\return Nothing.
	*/
	template <typename ComponentType>
	void AddComponentToEntity(const EntityID &p_EntityID, std::shared_ptr<ComponentType> p_Component) {
		auto entity = m_Entities.find(p_EntityID);
		if (entity != m_Entities.end()) {
			m_Components[typeid(ComponentType)].emplace(p_EntityID, p_Component);
		}
	}
	/*!
		\brief Adds a component to an entity.
		\tparam ComponentType the type of component to add.
		\param p_EntityName The name of the entity to add the component to.
		\param p_Component A shared pointer to the component that's being added to the entity.
		\return Nothing.
	*/
	template <typename ComponentType>
	void AddComponentToEntity(const std::string &p_EntityName, std::shared_ptr<ComponentType> p_Component) {
		AddComponentToEntity(m_StringHasher(p_EntityName), p_Component);
	}

	/*!
		\brief Gets an entity's component from the entity manager.
		\tparam ComponentType the type of component to get.
		\param p_Entity The ID of the entity to get the component from.
		\return Returns a shared pointer to the component of the given entity.
	*/
	template <typename ComponentType>
	std::shared_ptr<ComponentType> GetComponent(const EntityID &p_Entity) {
		auto componentType = m_Components.find(typeid(ComponentType));
		if (componentType != m_Components.end()) {
			auto entityWithComponent = componentType->second.find(p_Entity);
			if (entityWithComponent != componentType->second.end()) {
				return std::static_pointer_cast<ComponentType>(entityWithComponent->second);
			}
		}

		return nullptr;
	}
	/*!
		\brief Gets an entity's component from the entity manager.
		\tparam ComponentType the type of component to get.
		\param p_Entity The name of the entity to get the component from.
		\return Returns a shared pointer to the component of the given entity.
	*/
	template <typename ComponentType>
	std::shared_ptr<ComponentType> GetComponent(const std::string &p_Entity) {
		return GetComponent<ComponentType>(m_StringHasher(p_Entity));
	}

	/*!
		\brief Removes an entity's component from the entity manager.
		\tparam ComponentType the type of component to delete.
		\param p_EntityID The ID of the entity to remove the component from.
		\return Nothing.
	*/
	template <typename ComponentType>
	void DeleteComponent(const EntityID &p_EntityID) {
		auto entity = m_Entities.find(p_EntityID);
		if (entity != m_Entities.end()) {
			m_Components[typeid(ComponentType)].erase(p_EntityID);
		}
	}
	/*!
		\brief Removes an entity's component from the entity manager.
		\tparam ComponentType the type of component to delete.
		\param p_EntityName The name of the entity to remove the component from.
		\return Nothing.
	*/
	template <typename ComponentType>
	void DeleteComponent(const std::string &p_EntityName) {
		DeleteComponent<ComponentType>(m_StringHasher(p_EntityName));
	}

	/*!
		\brief Removes an entity to the entity manager.
		\param p_EntityID The ID of the entity to remove.
		\return Nothing.
	*/
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
	/*!
		\brief Removes an entity to the entity manager.
		\param p_EntityName The name of the entity to remove.
		\return Nothing.
	*/
	void DeleteEntity(const std::string &p_EntityName) {
		DeleteEntity(m_StringHasher(p_EntityName));
	}

	/*!
		\brief Adds an entity system to the entity manager.
		\param p_EntitySystem The entity system to add.
		\return Nothing.
	*/
	void AddSystem(std::shared_ptr<EntitySystem> p_EntitySystem) {
		m_Systems.push_back(p_EntitySystem);
	}

	/*!
		\brief Updates all of the entity systems.
		\param p_DeltaTime the time passed since the previous frame.
		\return Nothing.
	*/
	void UpdateSystems(float p_DeltaTime) {
		for (auto &entitySystem : m_Systems) {
			entitySystem->Update(p_DeltaTime);
		}
	}

	/*!
		\brief Renders all of the entity systems.
		\return Nothing.
	*/
	void RenderSystems() {
		for (auto &entitySystem : m_Systems) {
			entitySystem->Render();
		}
	}

	/*!
		\brief Clears the entity manager's data.
		\return Nothing.
	*/
	void Clear() {
		m_Entities.clear();
		m_Components.clear();
		m_Systems.clear();

		m_Instantiated = false;
	}
};