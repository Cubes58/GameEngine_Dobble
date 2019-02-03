#pragma once

#include <math.h>

template<class T>
class Vector2D {
protected:
	T m_XValue;
	T m_YValue;
public:
	Vector2D() : m_XValue(T(0)), m_YValue(T(0)) {}
	Vector2D(const T &p_Values) {
		m_XValue = p_Values; m_YValue = p_Values;
	}
	Vector2D(const T &p_XValue, const T &p_YValue) {
		m_XValue = p_XValue; m_YValue = p_YValue;
	}

	void SetX(const T &p_XValue) {
		m_XValue = p_XValue;
	}
	void SetY(const T &p_YValue) {
		m_YValue = p_YValue;
	}
	void Set(const T &p_XValue, const T &p_YValue) {
		m_XValue = p_XValue; m_YValue = p_YValue;
	}
	void Set(const Vector2D &p_Vector) {
		m_XValue = p_Vector.m_XValue; m_YValue = p_Vector.m_YValue;
	}

	T X() const {
		return m_XValue;
	}
	T Y() const {
		return m_YValue;
	}

	Vector2D operator+(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue + p_Vector.m_XValue, m_YValue + p_Vector.m_YValue);
	}
	Vector2D operator-(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue - p_Vector.m_XValue, m_YValue - p_Vector.m_YValue);
	}
	Vector2D operator*(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue * p_Vector.m_XValue, m_YValue * p_Vector.m_YValue);
	}
	Vector2D operator/(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue / p_Vector.m_XValue, m_YValue / p_Vector.m_YValue);
	}

	void operator=(const Vector2D &p_Vector) {
		m_XValue = p_Vector.m_XValue; m_YValue = p_Vector.m_YValue;
	}

	bool operator==(const Vector2D &p_Vector) const {
		return ((m_XValue == p_Vector.m_XValue) && (m_YValue == p_Vector.m_YValue));
	}
	bool operator!=(const Vector2D &p_Vector) const {
		return !((m_XValue == p_Vector.m_XValue) && (m_YValue == p_Vector.m_YValue));
	}

	// Negate both the x and y values.
	Vector2D operator-() const {
		return Vector2D(-m_XValue, -m_YValue);
	}

	// Scale both the x and y values.
	Vector2D operator*(const T &p_Scalar) const {
		return Vector2D(m_XValue * p_Scalar, m_YValue * p_Scalar);
	}
	Vector2D operator/(const T &p_Scalar) const {
		return Vector2D(m_XValue / p_Scalar, m_YValue / p_Scalar);
	}

	T DotProduct(const Vector2D &p_Vector) const {
		return ((m_XValue * p_Vector.m_XValue) + (m_YValue * p_Vector.m_YValue));
	}
	T CrossProduct(const Vector2D &p_Vector) const {
		return ((m_XValue * p_Vector.m_YValue) - (m_YValue * p_Vector.m_XValue));
	}
	T Magnitude() const {
		return sqrt(m_XValue * m_XValue + m_YValue * m_YValue);
	}

	Vector2D Normal() const {
		T magnitude = Magnitude();
		if (magnitude == 0)
			return Vector2D<T>(m_XValue, m_YValue);

		return Vector2D(m_XValue / magnitude, m_YValue / magnitude);
	}
	Vector2D Perpendicular() const {
		return Vector2D(m_YValue, -m_XValue);
	}
};