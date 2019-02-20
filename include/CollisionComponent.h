#pragma once

#include <unordered_map>
#include <vector>

#include "Component.h"
#include "Vector2D.h"

struct CollisionComponent : public Component {
	std::unordered_map<unsigned int, std::vector<Vector2D<float>>> m_CollisionVectors;
	std::vector<bool> m_ReversePosition;
	std::vector<bool> m_WithinCard;

	CollisionComponent(unsigned int p_NumberOfCirclesPerCard) {
		m_ReversePosition.reserve(p_NumberOfCirclesPerCard);
		m_WithinCard.reserve(p_NumberOfCirclesPerCard);
		for (unsigned int i = 0; i < p_NumberOfCirclesPerCard; ++i) {
			m_CollisionVectors.emplace(i, std::vector<Vector2D<float>>());
			m_ReversePosition.emplace_back(false);
			m_WithinCard.emplace_back(false);
		}
	}
};