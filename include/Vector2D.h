/**
@file Vector2D.h
@brief A 2D vector class.
*/
#pragma once

#include <math.h>

/*! \class Vector2D
	\brief A 2D vector class.
	\tparam T the type of values the vector will store.
*/

template<class T>
class Vector2D {
protected:
	T m_XValue;	//!< The X value of the 2D vector.
	T m_YValue;	//!< The Y value of the 2D vector.

public:
	/*!
		\brief Constructor.
	*/
	Vector2D() : m_XValue(T(0)), m_YValue(T(0)) {}
	/*!
		\brief Constructor.
		\param p_Values The value to set the two values stored inside of the vector. 
	*/
	Vector2D(const T &p_Values) {
		m_XValue = p_Values; m_YValue = p_Values;
	}
	/*!
		\brief Constructor.
		\param p_XValue The value to set the vector's X value.
		\param p_YValue The value to set the vector's Y value.
	*/
	Vector2D(const T &p_XValue, const T &p_YValue) {
		m_XValue = p_XValue; m_YValue = p_YValue;
	}
	/*!
		\brief Destructor.
	*/
	~Vector2D() = default;

	/*!
		\brief Sets the X value of the vector.
		\param p_XValue The vectors the X value.
		\return Nothing.
	*/
	void SetX(const T &p_XValue) {
		m_XValue = p_XValue;
	}
	/*!
		\brief Sets the Y value of the vector.
		\param p_YValue The vectors the Y value.
		\return Nothing.
	*/
	void SetY(const T &p_YValue) {
		m_YValue = p_YValue;
	}
	/*!
		\brief Sets the X and Y value of the vector.
		\param p_XValue The vectors the X value.
		\param p_YValue The vectors the Y value.
		\return Nothing.
	*/
	void Set(const T &p_XValue, const T &p_YValue) {
		m_XValue = p_XValue; m_YValue = p_YValue;
	}
	/*!
		\brief Sets the X and Y value of the vector.
		\param p_Vector Replaces this vector's values with the one passed in.
		\return Nothing.
	*/
	void Set(const Vector2D &p_Vector) {
		m_XValue = p_Vector.m_XValue; m_YValue = p_Vector.m_YValue;
	}

	/*!
		\brief Gets the X value, of the vector.
		\return Returns the X value, of the vector.
	*/
	T X() const {
		return m_XValue;
	}
	/*!
		\brief Gets the Y value, of the vector.
		\return Returns the Y value, of the vector.
	*/
	T Y() const {
		return m_YValue;
	}

	/*!
		\brief Adds two vectors together.
		\param p_Vector The vector to add.
		\return Returns the value of the addition.
	*/
	Vector2D operator+(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue + p_Vector.m_XValue, m_YValue + p_Vector.m_YValue);
	}
	/*!
		\brief Adds two vectors together, and assigns the value of the addition to this vector.
		\param p_Vector The vector to add.
		\return Returns the value of the addition.
	*/
	Vector2D& operator+=(const Vector2D& p_Vector) {
		*this = *this + p_Vector;
		return *this;
	}
	/*!
		\brief Subtracts two vectors from each other, and assigns the value to this vector.
		\param p_Vector The vector to subtract.
		\return Returns the value of the subtraction.
	*/
	Vector2D& operator-=(const Vector2D& p_Vector) {
		*this = *this - p_Vector;
		return *this;
	}
	/*!
		\brief Subtracts two vectors from each other.
		\param p_Vector The vector to subtract.
		\return Returns the value of the subtraction.
	*/
	Vector2D operator-(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue - p_Vector.m_XValue, m_YValue - p_Vector.m_YValue);
	}
	/*!
		\brief Multiplies this vector with another.
		\param p_Vector The vector to multiply by.
		\return Returns the value of the multiplication.
	*/
	Vector2D operator*(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue * p_Vector.m_XValue, m_YValue * p_Vector.m_YValue);
	}
	/*!
		\brief Divides this vector by another.
		\param p_Vector The vector to divide by.
		\return Returns the value of the division.
	*/
	Vector2D operator/(const Vector2D &p_Vector) const {
		return Vector2D(m_XValue / p_Vector.m_XValue, m_YValue / p_Vector.m_YValue);
	}

	/*!
		\brief Assigns the values from a vector to this vector.
		\param p_Vector The vector to assign the values from.
		\return Nothing.
	*/
	void operator=(const Vector2D &p_Vector) {
		m_XValue = p_Vector.m_XValue; m_YValue = p_Vector.m_YValue;
	}

	/*!
		\brief Checks if two vectors are identical.
		\param p_Vector The vector to check with.
		\return Returns true if the two vectors are identical, otherwise false.
	*/
	bool operator==(const Vector2D &p_Vector) const {
		return ((m_XValue == p_Vector.m_XValue) && (m_YValue == p_Vector.m_YValue));
	}
	/*!
		\brief Checks if two vectors are not identical.
		\param p_Vector The vector to check with.
		\return Returns true if the two vectors are not identical, otherwise false.
	*/
	bool operator!=(const Vector2D &p_Vector) const {
		return !((m_XValue == p_Vector.m_XValue) && (m_YValue == p_Vector.m_YValue));
	}
	/*!
		\brief Checks if one vector is greater than or equal to another.
		\param p_Vector The vector to check with.
		\return Returns true if the vector is greater than or equal to the other vector.
	*/
	bool operator>=(const Vector2D &p_Vector) const {
		return (m_XValue >= p_Vector.m_XValue) && (m_YValue >= p_Vector.m_YValue);
	}

	/*!
		\brief Negate both the x and y values.
		\return Returns the negated vector.
	*/
	Vector2D operator-() const {
		return Vector2D(-m_XValue, -m_YValue);
	}

	/*!
		\tparam T The type of the vector values.
		\brief Scale both the x and y values.
		\param p_Scalar The scalar to multiply by.
		\return Returns the scaled vector.
	*/
	Vector2D operator*(const T &p_Scalar) const {
		return Vector2D(m_XValue * p_Scalar, m_YValue * p_Scalar);
	}
	/*!
		\tparam T The type of the vector values.
		\brief Divides both the x and y values.
		\param p_Scalar The scalar to divide by.
		\return Returns the divided vector.
	*/
	Vector2D operator/(const T &p_Scalar) const {
		return Vector2D(m_XValue / p_Scalar, m_YValue / p_Scalar);
	}

	/*!
		\brief Calculates the dot product, with another vector.
		\param p_Vector The vector to dot product with this vector.
		\return Returns the dot product between this vector and the one passed in.
	*/
	T DotProduct(const Vector2D &p_Vector) const {
		return ((m_XValue * p_Vector.m_XValue) + (m_YValue * p_Vector.m_YValue));
	}
	/*!
		\brief Calculates the cross product, with another vector.
		\param p_Vector The vector to cross product this vector with.
		\return Returns the cross product between this vector and the one passed in.
	*/
	T CrossProduct(const Vector2D &p_Vector) const {
		return ((m_XValue * p_Vector.m_YValue) - (m_YValue * p_Vector.m_XValue));
	}
	/*!
		\brief Calculates the magnitude of the vector.
		\return Returns the magnitude of the vector.
	*/
	T Magnitude() const {
		return sqrt(m_XValue * m_XValue + m_YValue * m_YValue);
	}

	/*!
		\brief Calculates the normal of the vector.
		\return Returns the normal, of the vector.
	*/
	Vector2D Normal() const {
		T magnitude = Magnitude();
		if (magnitude == 0)
			return Vector2D<T>(m_XValue, m_YValue);

		return Vector2D(m_XValue / magnitude, m_YValue / magnitude);
	}
	/*!
		\brief Gets the perpendicular of the vector.
		\return Returns the perpendicular of the vector.
	*/
	Vector2D Perpendicular() const {
		return Vector2D(m_YValue, -m_XValue);
	}
};

using Vector2Df = Vector2D<float>;	//!< Using Vector2Df as Vector2D<float>
using Vector2Dd = Vector2D<double>;	//!< Using Vector2Dd as Vector2D<double>
using Vector2Di = Vector2D<int>;	//!< Using Vector2Di as Vector2D<int>
using Vector2Du = Vector2D<unsigned int>;	//!< Using Vector2Du as Vector2D<unsigned int>
using Vector2Dl = Vector2D<long>;	//!< Using Vector2Dl as Vector2D<long>