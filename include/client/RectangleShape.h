#pragma once

#include "Shape.h"

class RectangleShape : public Shape {
private:
	Vector2Df m_Size;

	void GenerateVertexData();

public:
	RectangleShape(const Vector2Df &p_Position = Vector2Df(0.0f, 0.0f), 
		const Vector2Df &p_Size = Vector2Df(10.0f, 10.0f), Texture2D *p_Texture = nullptr);
	~RectangleShape() = default;

	void Render(Shader &p_Shader) override;

	const Vector2Df &GetSize() const {
		return m_Size;
	}
	void SetSize(const Vector2Df &p_Size) {
		m_Size = p_Size;
	}
};