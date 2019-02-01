#pragma once

#include <array>

#include "Component.h"
#include "Vector2D.h"

struct CircleTransformData {
	Vector2D<float> m_Position;
	float m_Radius;
	float m_Rotation;

	CircleTransformData() : m_Position(0.0f, 0.0f), m_Radius(0.0f), m_Rotation(0.0f) {}
	CircleTransformData(Vector2D<float> p_Position) : m_Position(p_Position), m_Radius(0.0f), m_Rotation(0.0f) {}
	CircleTransformData(Vector2D<float> p_Position, float p_Radius) : m_Position(p_Position), m_Radius(p_Radius), m_Rotation(0.0f) {}
	CircleTransformData(Vector2D<float> p_Position, float p_Radius, float p_Rotation) : m_Position(p_Position), m_Radius(p_Radius), m_Rotation(p_Rotation) {}
};

struct TransformComponent : public Component {
	std::array<CircleTransformData, s_NUMBER_OF_CIRCLES_PER_CARD> m_CircleTransforms;

	TransformComponent() = default;
	TransformComponent(std::array<CircleTransformData, s_NUMBER_OF_CIRCLES_PER_CARD> p_CirclePositions) : m_CircleTransforms(p_CirclePositions) {}
};