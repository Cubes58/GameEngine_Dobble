#pragma once

#include <glm/vec4.hpp>

#include "Vector2D.h"
#include "Texture2D.h"

class Shader;

class Shape {
protected:
	glm::vec4 m_Colour;
	Vector2Df m_Position;
	float m_Rotation = 0.0f;
	float m_ColourTextureMixValue = 0.0f;
	unsigned int m_VBO;
	unsigned int m_VAO;
	bool m_UsingColourAsTextureMask = false;
	Texture2D *m_Texture;

public:
	Shape() = default;
	Shape(const Vector2Df &p_Position) : m_Position(p_Position) { }
	Shape(Texture2D *p_Texture) : m_Texture(p_Texture) { }
	Shape(const glm::vec4 &p_Colour) : m_Colour(p_Colour) { }
	Shape(const Vector2Df &p_Position, Texture2D *p_Texture = nullptr, float p_Rotation = 0.0f, const glm::vec4 &p_Colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		: m_Position(p_Position), m_Texture(p_Texture), m_Rotation(p_Rotation), m_Colour(p_Colour) { }
	~Shape() {
		gl::DeleteBuffers(1, &m_VBO);
		gl::DeleteVertexArrays(1, &m_VAO);
	}

	virtual void Render(Shader &p_Shader) = 0;

	unsigned int GetVAOID() const {
		return m_VAO;
	}

	const Vector2Df &GetPosition() const {
		return m_Position;
	}
	void SetPosition(const Vector2Df &p_Position) {
		m_Position = p_Position;
	}

	float GetRotation() const {
		return m_Rotation;
	}
	void SetRotation(float p_Rotation) {
		m_Rotation = p_Rotation;
	}

	const Texture2D *GetTexture() const {
		return m_Texture;
	}
	void SetTexture(Texture2D *p_Texture) {
		m_Texture = p_Texture;
	}

	const glm::vec4 &GetColour() const {
		return m_Colour;
	}
	void SetColour(const glm::vec4 &p_Colour) {
		m_Colour = p_Colour;
	}

	float GetColourTextureMixValue() const {
		return m_ColourTextureMixValue;
	}
	void SetColourTextureMixValue(float p_ColourTextureMixValue) {
		m_ColourTextureMixValue = p_ColourTextureMixValue;
	}

	bool GetUsingColourAsTextureMask() const {
		return m_UsingColourAsTextureMask;
	}
	void SetUsingColourAsTextureMask(bool p_UsingColourAsTextureMask) {
		m_UsingColourAsTextureMask = p_UsingColourAsTextureMask;
	}
};