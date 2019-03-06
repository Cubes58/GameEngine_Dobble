#pragma once

#include <vector>

#include "Component.h"
#include "Vector2D.h"
#include "PacketTypes.h"

struct CircleTransformData {
	Vector2Df m_Position;
	float m_Radius;
	float m_Rotation;

	CircleTransformData() : m_Position(0.0f, 0.0f), m_Radius(0.0f), m_Rotation(0.0f) {}
	CircleTransformData(Vector2Df p_Position) : m_Position(p_Position), m_Radius(0.0f), m_Rotation(0.0f) {}
	CircleTransformData(Vector2Df p_Position, float p_Radius) : m_Position(p_Position), m_Radius(p_Radius), m_Rotation(0.0f) {}
	CircleTransformData(Vector2Df p_Position, float p_Radius, float p_Rotation) : m_Position(p_Position), m_Radius(p_Radius), m_Rotation(p_Rotation) {}

	bool operator==(const CircleTransformData &p_CircleTransformData) {
		return (this->m_Position == p_CircleTransformData.m_Position 
			&& this->m_Radius == p_CircleTransformData.m_Radius 
			&& this->m_Rotation == p_CircleTransformData.m_Rotation);
	}
	bool operator!=(const CircleTransformData &p_CircleTransformData) {
		return !(*this == p_CircleTransformData);
	}
};

struct TransformComponent : public Component {
	std::vector<CircleTransformData> m_CircleTransforms;

	TransformComponent() = default;
	TransformComponent(unsigned int p_NumberOfCirclesPerCard) {
		m_CircleTransforms.reserve(p_NumberOfCirclesPerCard);
		for (unsigned int i = 0; i < p_NumberOfCirclesPerCard; i++) {
			m_CircleTransforms.emplace_back(CircleTransformData());
		}
	}
	TransformComponent(const std::vector<CircleTransformData> &p_CirclePositions) 
		: m_CircleTransforms(p_CirclePositions) {}
};

sf::Packet &operator<<(sf::Packet &p_Packet, TransformComponent &p_TransformComponent);
sf::Packet &operator>>(sf::Packet &p_Packet, TransformComponent &p_TransformComponent);