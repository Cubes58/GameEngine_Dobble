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

				// Centre of the card background.
				Vector2D<float> centrePoint(transformComponent->m_CircleTransforms[0].m_Position);

				std::array<CircleTransformData, s_NUMBER_OF_CIRCLES_PER_CARD> &circleTransforms = transformComponent->m_CircleTransforms;
				std::array<CircleTransformData, s_NUMBER_OF_CIRCLES_PER_CARD> &previousCircleTransforms = transformComponent->m_PreviousCircleTransforms;
				for (int i = 1; i < s_NUMBER_OF_CIRCLES_PER_CARD; ++i) {
					Vector2D<float> moveVector((circleTransforms[i].m_Position - centrePoint) * s_MOVE_SPEED);

					// Check whether the symbol is within the card's area. If it isn't move it, towards the centre point.
					if (!collisionComponent->m_WithinCard[i]) {
						// If the symbol isn't colliding with another symbol and isn't at the centre of the card, then move it closer.
						if (!collisionComponent->m_ReversePosition[i] && circleTransforms[i].m_Position != centrePoint) {
							previousCircleTransforms[i].m_Position = circleTransforms[i].m_Position;
							// Move it towards the centre.
							circleTransforms[i].m_Position = circleTransforms[i].m_Position - moveVector;
						}
						else if (!collisionComponent->m_ReversePosition[i]) {

						}


					}


					if (!collisionComponent->m_WithinCard[i]) {
						circleTransforms[i].m_Position = circleTransforms[i].m_Position - moveVector;
					}
				}
			}
		}
	}
};