#pragma once

#include "Shape.h"

class RectangleShape : public Shape {
private:
	Vector2D<float> m_Size;

	void GenerateVertexData();

public:
	RectangleShape(const Vector2D<float> &p_Position = Vector2D<float>(0.0f, 0.0f), 
		const Vector2D<float> &p_Size = Vector2D<float>(10.0f, 10.0f), Texture2D *p_Texture = nullptr);
	~RectangleShape() = default;

	void Render(Shader &p_Shader) override;

	const Vector2D<float> &GetSize() const {
		return m_Size;
	}
	void SetSize(const Vector2D<float> &p_Size) {
		m_Size = p_Size;
	}
};