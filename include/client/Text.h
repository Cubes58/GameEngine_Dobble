#pragma once

#include <string>

#include <glm/vec3.hpp>

#include "Vector2D.h"

struct Text {
	std::string m_Text;
	glm::vec3 m_Colour;
	Vector2Df m_Position;
	float m_Size;

	Text() = default;
	Text(const std::string &p_Text, const Vector2Df &p_Position, float p_Size, const glm::vec3 &p_Colour)
		: m_Text(p_Text), m_Position(p_Position), m_Size(p_Size), m_Colour(p_Colour) {  }
	~Text() = default;

	virtual void Update() {
		return;
	}
};

template<typename Type>
struct ReferenceText : Text {
	Type &m_ReferenceData;
	std::string m_OriginalText;

	ReferenceText(const std::string &p_Text, Type &p_ReferenceData, const Vector2Df &p_Position, float p_Size, const glm::vec3 &p_Colour)
		: Text(p_Text, p_Position, p_Size, p_Colour), m_ReferenceData(p_ReferenceData), m_OriginalText(p_Text) {
	}
	~ReferenceText() = default;

	virtual void Update() {
		m_Text = m_OriginalText + std::to_string((int)m_ReferenceData);
	}
};