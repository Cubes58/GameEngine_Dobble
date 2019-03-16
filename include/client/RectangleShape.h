/**
@file RectangleShape.h
@brief Manages the properties required for a rectangle shape.
*/
#pragma once

#include "Shape.h"

/*! \class RectangleShape
	\brief Manages the properties required for a rectangle shape.
*/
class RectangleShape : public Shape {
private:
	Vector2Df m_Size;	//!< The size of the rectangle.

	/*!
		\brief Generates the vertex data, for the rectangle shape.
		\return Nothing.
	*/
	void GenerateVertexData();

public:
	/*!
		\brief Constructor.
		\param p_Position The position of the rectangle.
		\param p_Size The size of the rectangle.
		\param p_Texture The texture used by the rectangle.
	*/
	RectangleShape(const Vector2Df &p_Position = Vector2Df(0.0f, 0.0f), 
		const Vector2Df &p_Size = Vector2Df(10.0f, 10.0f), Texture2D *p_Texture = nullptr);
	~RectangleShape() = default;	//!< Destructor.

	/*!
		\brief Renders the rectangle.
		\param p_Shader The shader used to render the rectangle.
		\return Nothing.
	*/
	void Render(Shader &p_Shader) override;

	/*!
		\brief Gets the size of the rectangle.
		\return The size of the rectangle.
	*/
	const Vector2Df &GetSize() const {
		return m_Size;
	}
	/*!
		\brief Sets the size of the rectangle.
		\param p_Size the rectangle's new size.
		\return Nothing.
	*/
	void SetSize(const Vector2Df &p_Size) {
		m_Size = p_Size;
	}
};