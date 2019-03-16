/**
@file EntitySystem.h
@brief A base class for entity systems.
*/
#pragma once

#include <typeindex>
#include <map>
#include <set>
#include <memory>

using EntityID = std::size_t;	//!< Using EntityID as std::size_t
struct Component;

/*! \class EntitySystem
	\brief A base class for entity systems.
*/

class EntitySystem {
protected:
	std::set<EntityID> *m_Entities;	//!< The entites stored in the entity manager (direct access).

	/*!
		\brief Get many components of the same type, for every entity that has that component type.
		\param p_TypeIndex the component type being requested, from the entities.
		\return Pointer to a map of entities and their components of a certain type.
	*/
	std::map<EntityID, std::shared_ptr<Component>> *GetComponentArray(std::type_index p_TypeIndex);

public:
	EntitySystem();	//!< Constructor.
	virtual ~EntitySystem() = default;	//!< Destructor.

	/*!
		\brief Update the entity system.
		\param p_DeltaTime the time passed.
		\return Nothing.
	*/
	virtual void Update(float p_DeltaTime) {}
	/*!
		\brief Render the entity system.
		\return Nothing.
	*/
	virtual void Render() {}
};