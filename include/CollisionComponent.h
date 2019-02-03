#pragma once

#include <unordered_map>
#include <vector>
#include <array>

#include "Component.h"
#include "Vector2D.h"

struct CollisionComponent : public Component {
	std::unordered_map<unsigned int, std::vector<Vector2D<float>>> m_CollisionVectors;
	std::array<bool, s_NUMBER_OF_CIRCLES_PER_CARD> m_ReversePosition { false, false, false, false, false, false, false, false, false };

	CollisionComponent() {
		for (int i = 0; i < s_NUMBER_OF_CIRCLES_PER_CARD; ++i) {
			m_CollisionVectors.emplace(i, std::vector<Vector2D<float>>());
		}
	}
};