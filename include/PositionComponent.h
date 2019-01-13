#pragma once

#include "Component.h"

struct PositionComponent : public Component {
	float m_XPosition;
	float m_YPosition;

	float m_Width;
	float m_Height;

	float m_Rotation;
};