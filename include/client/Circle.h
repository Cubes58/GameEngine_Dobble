#pragma once

#include "Shape.h"

class Circle : public Shape {
private:
	unsigned int m_NumberOfFaces;
	unsigned int m_NumberOfVerticesToDraw;
	float m_Radius;

public:
	Circle(const Vector2D<float> &p_Position = Vector2D<float>(0.0f, 0.0f), unsigned int p_NumberOfFaces = 64, 
		float p_Radius = 1.0f, Texture2D *p_Texture = nullptr);
	~Circle() = default;

	void Render(Shader &p_Shader) override;

	unsigned int GetNumberOfFaces() const {
		return m_NumberOfFaces;
	}
	void GenerateFaces(unsigned int p_NumberOfFaces);

	float GetRadius() const {
		return m_Radius;
	}
	void SetRadius(float p_Radius) {
		m_Radius = p_Radius;
	}
};