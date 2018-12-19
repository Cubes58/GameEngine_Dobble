#pragma once

#include <unordered_map>
#include <memory>
#include <any>

#include "Entity.h"

class EntitySystem {
public:
	virtual ~EntitySystem() = default;

	virtual void HandleEvent(std::unordered_map<EntityID, std::shared_ptr<Entity>> p_Entities, std::any p_Event) = 0;
	virtual void Update(std::unordered_map<EntityID, std::shared_ptr<Entity>> p_Entities, float p_DeltaTime) = 0;
	virtual void Draw(std::unordered_map<EntityID, std::shared_ptr<Entity>> p_Entities) = 0;
};