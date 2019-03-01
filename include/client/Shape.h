#pragma once

#include <glm/vec4.hpp>

#include "Vector2D.h"
#include "Texture2D.h"

class Shader;

class Shape {
protected:
	glm::vec4 m_Colour;
	Vector2D<float> m_Position;
	float m_Rotation = 0.0f;
	unsigned int m_VBO;
	unsigned int m_VAO;
	Texture2D *m_Texture;

public:
	Shape() = default;
	Shape(const Vector2D<float> &p_Position) : m_Position(p_Position) { }
	Shape(Texture2D *p_Texture) : m_Texture(p_Texture) { }
	Shape(const glm::vec4 &p_Colour) : m_Colour(p_Colour) { }
	Shape(const Vector2D<float> &p_Position, Texture2D *p_Texture = nullptr, float p_Rotation = 0.0f, const glm::vec4 &p_Colour = glm::vec4(255, 255, 255, 128))
		: m_Position(p_Position), m_Texture(p_Texture), m_Rotation(p_Rotation), m_Colour(p_Colour) { }
	~Shape() {
		gl::DeleteBuffers(1, &m_VBO);
		gl::DeleteVertexArrays(1, &m_VAO);
	}

	virtual void Render(Shader &p_Shader) = 0;

	const Vector2D<float> &GetPosition() const {
		return m_Position;
	}
	void SetPosition(const Vector2D<float> &p_Position) {
		m_Position = p_Position;
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
};