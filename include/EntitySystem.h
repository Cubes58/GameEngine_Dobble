#pragma once

#include <unordered_map>
#include <memory>
#include <any>

#include "Entity.h"
#include "ComponentContainer.h"

class EntityManager;

class EntitySystem {
protected:
	ComponentContainer &GetComponentContainer();

public:
	EntitySystem() = default;
	virtual ~EntitySystem() = default;

	virtual void Process(const EntityID &p_Entity) = 0;
};
