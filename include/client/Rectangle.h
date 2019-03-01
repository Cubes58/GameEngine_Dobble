#pragma once

#include "Shape.h"

class Rectangle : public Shape {
private:
	Vector2D<float> m_Size;

public:
	Rectangle();
	~Rectangle();


	const Vector2D<float> &GetSize() const {
		return m_Size;
	}
	void SetSize(const Vector2D<float> &p_Size) {
		m_Size = p_Size;
	}
};