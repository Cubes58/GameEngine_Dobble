#pragma once

#include "EntitySystem.h"
#include "EntityManager.h"
#include "Vector2D.h"

#include "CollisionComponent.h"
#include "TransformComponent.h"

class CollisionSystem : public EntitySystem {
private:
	std::map<EntityID, std::shared_ptr<Component>> *m_CollisionComponents;
	std::map<EntityID, std::shared_ptr<Component>> *m_TransformComponents;

	bool operator()() {

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
			if (potentialCollisionComponent != m_CollisionComponents->end() && potentialTransformComponent != m_TransformComponents->end()) {
				std::shared_ptr<CollisionComponent> renderComponent = std::dynamic_pointer_cast<CollisionComponent>(potentialCollisionComponent->second);
				std::shared_ptr<TransformComponent> transformComponent = std::dynamic_pointer_cast<TransformComponent>(potentialTransformComponent->second);



			}
		}
	}

	virtual void Render(Window &p_Window) override {
		// Render debug info.
	}
};