#pragma once

#include <vector>

#include "Component.h"
#include "Vector2D.h"

struct CollisionComponent : public Component {
	std::vector<Vector2D<float>> m_CollisionPoints;
	bool m_HasCollided = false;
};