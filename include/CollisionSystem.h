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

	bool m_SymbolsSuccessfullyWithinCards = false;

	// Circle/Circle collision.
	bool operator()(Vector2D<float> p_EntityOnePosition, float p_EntityOneRadius, Vector2D<float> p_EntityTwoPosition, float p_EntityTwoRadius, std::vector<Vector2D<float>> &p_CollisionVectors) {
		Vector2D<float> distance(p_EntityOnePosition.X() - p_EntityTwoPosition.X(), p_EntityOnePosition.Y() - p_EntityTwoPosition.Y());
		float radii = p_EntityOneRadius + p_EntityTwoRadius;
		
		if ((distance.X() * distance.X()) + (distance.Y() * distance.Y()) < radii * radii) {
			p_CollisionVectors.emplace_back(distance.Normal());
			return true;
		}

		return false;
	}

	bool WithinCardAreaCircleCircle(Vector2D<float> p_CardCentrePosition, float p_CardRadius, Vector2D<float> p_EntityOnePosition, float p_EntityOneRadius) {
		Vector2D<float> distance(p_CardCentrePosition - p_EntityOnePosition);
		float squareDist = distance.DotProduct(distance);
		float radiiSum = p_CardRadius - p_EntityOneRadius;
		
		return squareDist < radiiSum * radiiSum;;

		/* WORKS - JUST EXPENSIVE (USES SQRT).
		float distance = std::sqrt(((p_CardCentrePosition.X() - p_EntityOnePosition.X()) * (p_CardCentrePosition.X() - p_EntityOnePosition.X())) + 
			(p_CardCentrePosition.Y() - p_EntityOnePosition.Y()) * ((p_CardCentrePosition.Y() - p_EntityOnePosition.Y())));
		distance += (p_EntityOneRadius + 1.5f);

		if (p_CardRadius >= distance) {
			return true;
		}
		return false;
		*/
	}

public:
	CollisionSystem() {
		m_CollisionComponents = GetComponentArray(typeid(CollisionComponent));
		m_TransformComponents = GetComponentArray(typeid(TransformComponent));
	}
	~CollisionSystem() = default;

	virtual void Update(float p_DeltaTime) override {
		m_SymbolsSuccessfullyWithinCards = true;
		for (const auto &entity : *m_Entities) {
			auto potentialCollisionComponent = m_CollisionComponents->find(entity);
			auto potentialTransformComponent = m_TransformComponents->find(entity);

			if (potentialTransformComponent != m_TransformComponents->end()) {
				std::shared_ptr<CollisionComponent> collisionComponent = std::static_pointer_cast<CollisionComponent>(potentialCollisionComponent->second);
				std::shared_ptr<TransformComponent> transformComponent = std::static_pointer_cast<TransformComponent>(potentialTransformComponent->second);
				
				collisionComponent->m_WithinCard[0] = true;
				for (int i = 1; i < s_NUMBER_OF_CIRCLES_PER_CARD; ++i) {
					auto collisionVector = collisionComponent->m_CollisionVectors.find(i);
					for (int j = 1; j < s_NUMBER_OF_CIRCLES_PER_CARD; ++j) {
						if (i == j)
							continue;

						collisionComponent->m_ReversePosition[i] = operator()(transformComponent->m_CircleTransforms[i].m_Position, transformComponent->m_CircleTransforms[i].m_Radius,
							transformComponent->m_CircleTransforms[j].m_Position, transformComponent->m_CircleTransforms[j].m_Radius, collisionVector->second);

						if (!collisionComponent->m_ReversePosition[i])
							m_SymbolsSuccessfullyWithinCards = false;
					}

					collisionComponent->m_WithinCard[i] = WithinCardAreaCircleCircle(transformComponent->m_CircleTransforms[0].m_Position, transformComponent->m_CircleTransforms[0].m_Radius,
						transformComponent->m_CircleTransforms[i].m_Position, transformComponent->m_CircleTransforms[i].m_Radius);
					
					if (!collisionComponent->m_WithinCard[i])
						m_SymbolsSuccessfullyWithinCards = false;
				}
			}
		}
	}
};