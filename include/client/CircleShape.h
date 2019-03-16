/**
@file CircleShape.h
@brief Manages the properties required for a circle shape.
*/
#pragma once

#include "Shape.h"

/*! \class CircleShape
	\brief Manages the properties required for a circle shape.
*/

class CircleShape : public Shape {
private:
	unsigned int m_NumberOfFaces;	//!< The number of faces the circle has.
	unsigned int m_NumberOfVerticesToDraw;	//!< The number of vertices used to draw the circle.
	float m_Radius;	//!< The circle's radius.

public:
	/*!
		\brief Constructor.
		\param p_Position The position of the circle.
		\param p_NumberOfFaces The number of faces the circle will have.
		\param p_Radius The radius of the circle.
		\param p_Texture The texture used by the circle.
	*/
	CircleShape(const Vector2Df &p_Position = Vector2Df(0.0f, 0.0f), unsigned int p_NumberOfFaces = 64, 
		float p_Radius = 1.0f, Texture2D *p_Texture = nullptr);
	~CircleShape() = default;	//!< Destructor.

	/*!
		\brief Renders the circle shape.
		\param p_Shader the shader used to render the circle.
		\return Nothing.
	*/
	void Render(Shader &p_Shader) override;

	/*!
		\brief Gets the number of faces the circle has.
		\return Returns the number of faces the circle has.
	*/
	unsigned int GetNumberOfFaces() const {
		return m_NumberOfFaces;
	}
	/*!
		\brief (Re)Generates the circle's faces.
		\param p_NumberOfFaces The number of faces the generates circle will have.
		\return Nothing.
	*/
	void GenerateFaces(unsigned int p_NumberOfFaces);

	/*!
		\brief Gets the circle radius.
		\return Returns the circle's radius.
	*/
	float GetRadius() const {
		return m_Radius;
	}
	/*!
		\brief Sets the circle's radius.
		\param p_Radius The circle's new radius.
		\return Nothing.
	*/
	void SetRadius(float p_Radius) {
		m_Radius = p_Radius;
	}
};