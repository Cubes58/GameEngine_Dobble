#pragma once

#include <math.h>

#include "EntitySystem.h"
#include "EntityManager.h"
#include "Vector2D.h"

#include "CollisionComponent.h"
#include "TransformComponent.h"

class CollisionSystem : public EntitySystem {
private:
	std::map<EntityID, std::shared_ptr<Component>> *m_CollisionComponents;
	std::map<EntityID, std::shared_ptr<Component>> *m_TransformComponents;

	bool operator()(Vector2D<float> p_EntityOnePosition, float p_EntityOneRadius, Vector2D<float> p_EntityTwoPosition, float p_EntityTwoRadius, std::vector<Vector2D<float>> &p_CollisionVectors) {
		Vector2D<float> distance(p_EntityOnePosition.X() - p_EntityTwoPosition.X(), p_EntityOnePosition.Y() - p_EntityTwoPosition.Y());
		float radii = p_EntityOneRadius + p_EntityTwoRadius;
		
		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < radii * radii) {
			p_CollisionVectors.emplace_back(distance.Normal());
			return true;
		}

		return false;
	}

public:
	CollisionSystem() {
		m_CollisionComponents = GetComponentArray(typeid(CollisionComponent));
		m_TransformComponents = GetComponentArray(typeid(TransformComponent));
	}
	~CollisionSystem() = default;

	virtual void Update(float p_DeltaTime) override {
		for (const auto &entity : *m_Entities) {
			auto potentialCollisionComponent = m_CollisionComponents->find(entity);
			auto potentialTransformComponent = m_TransformComponents->find(entity);

			if (potentialTransformComponent != m_TransformComponents->end()) {
				std::shared_ptr<CollisionComponent> collisionComponent = std::dynamic_pointer_cast<CollisionComponent>(potentialCollisionComponent->second);
				std::shared_ptr<TransformComponent> transformComponent = std::static_pointer_cast<TransformComponent>(potentialTransformComponent->second);

				for (int i = 1; i < s_NUMBER_OF_CIRCLES_PER_CARD; ++i) {
					auto collisionVector = collisionComponent->m_CollisionVectors.find(i);
					for (int j = 1; j < s_NUMBER_OF_CIRCLES_PER_CARD; ++j) {
						if (i == j)
							continue;
						else if (operator()(transformComponent->m_CircleTransforms[i].m_Position, transformComponent->m_CircleTransforms[i].m_Radius, 
							transformComponent->m_CircleTransforms[j].m_Position, transformComponent->m_CircleTransforms[j].m_Radius, collisionVector->second)) {
							collisionComponent->m_ReversePosition[i] = true;
						}
					}
				}
			}
		}
	}
};