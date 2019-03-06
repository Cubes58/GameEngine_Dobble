#pragma once

#include <math.h>
#include <array>

#include "EntitySystem.h"
#include "EntityManager.h"
#include "Vector2D.h"

#include "CollisionComponent.h"
#include "TransformComponent.h"

static constexpr auto s_MOVE_SPEED = 0.002f;

class MoveSymbolsSystem : public EntitySystem {
private:
	std::map<EntityID, std::shared_ptr<Component>> *m_CollisionComponents;
	std::map<EntityID, std::shared_ptr<Component>> *m_TransformComponents;

public:
	MoveSymbolsSystem() {
		m_CollisionComponents = GetComponentArray(typeid(CollisionComponent));
		m_TransformComponents = GetComponentArray(typeid(TransformComponent));
	}
	~MoveSymbolsSystem() = default;

	virtual void Update(float p_DeltaTime) override {
		for (const auto &entity : *m_Entities) {
			auto potentialCollisionComponent = m_CollisionComponents->find(entity);
			auto potentialTransformComponent = m_TransformComponents->find(entity);

			if (potentialTransformComponent != m_TransformComponents->end()) {
				std::shared_ptr<CollisionComponent> collisionComponent = std::static_pointer_cast<CollisionComponent>(potentialCollisionComponent->second);
				std::shared_ptr<TransformComponent> transformComponent = std::static_pointer_cast<TransformComponent>(potentialTransformComponent->second);

			}
		}
	}
};