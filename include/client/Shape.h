/**
@file Shape.h
@brief An abstract base class for 2D shapes.
*/
#pragma once

#include <glm/vec4.hpp>

#include "Vector2D.h"
#include "Texture2D.h"

class Shader;

/*! \class Shape
	\brief An abstract base class for 2D shapes.
*/

class Shape {
protected:
	glm::vec4 m_Colour;	//!< Colour/colour mask of the shape.
	Vector2Df m_Position;	//!< The position of the shape.
	float m_Rotation = 0.0f;	//!< The shape's rotation.
	float m_ColourTextureMixValue = 0.0f;	//!< How much the colour of the shape should mix in with the texture.
	unsigned int m_VBO;	//!< The shapes vertex VBO ID.
	unsigned int m_VAO;	//!< The shapes VAO ID.
	bool m_UsingColourAsTextureMask = false;	//!< Manages whether the colour is a mask, or not.
	Texture2D *m_Texture;	//!< A pointer to the texture used by the shape.

public:
	/*!
		\brief Constructor.
	*/
	Shape() = default;
	/*!
		\brief Constructor.
		\param p_Position The position of the shape.
	*/
	Shape(const Vector2Df &p_Position) : m_Position(p_Position) { }
	/*!
		\brief Constructor.
		\param p_Texture A pointer to the texture used by the shape.
	*/
	Shape(Texture2D *p_Texture) : m_Texture(p_Texture) { }
	/*!
		\brief Constructor.
		\param p_Colour The colour of the shape.
	*/
	Shape(const glm::vec4 &p_Colour) : m_Colour(p_Colour) { }
	/*!
		\brief Constructor.
		\param p_Position The position of the shape.
		\param p_Texture A pointer to the texture used by the shape.
		\param p_Rotation The shape's rotation (orientation).
		\param p_Colour The colour of the shape.
	*/
	Shape(const Vector2Df &p_Position, Texture2D *p_Texture = nullptr, float p_Rotation = 0.0f, const glm::vec4 &p_Colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
		: m_Position(p_Position), m_Texture(p_Texture), m_Rotation(p_Rotation), m_Colour(p_Colour) { }

	/*!
		\brief Destructor.
	*/
	~Shape() {
		gl::DeleteBuffers(1, &m_VBO);
		gl::DeleteVertexArrays(1, &m_VAO);
	}

	/*!
		\brief A pure virtual method, which derived classes will have to define, to render the shape.
		\param p_Shader The shader used, to render the shape.
		\return Nothing.
	*/
	virtual void Render(Shader &p_Shader) = 0;

	/*!
		\brief Gets the VAO ID.
		\return Returns the VAO ID.
	*/
	unsigned int GetVAOID() const {
		return m_VAO;
	}

	/*!
		\brief Gets the shape's position.
		\return Returns the shape's position.
	*/
	const Vector2Df &GetPosition() const {
		return m_Position;
	}
	/*!
		\brief Sets the shape's position.
		\param p_Position The shape's new position.
		\return Nothing.
	*/
	void SetPosition(const Vector2Df &p_Position) {
		m_Position = p_Position;
	}

	/*!
		\brief Gets the shape's rotation.
		\return Returns the shape's rotation.
	*/
	float GetRotation() const {
		return m_Rotation;
	}
	/*!
		\brief Sets the shape's rotation.
		\param p_Rotation The shape's new rotation.
		\return Nothing.
	*/
	void SetRotation(float p_Rotation) {
		m_Rotation = p_Rotation;
	}

	/*!
		\brief Get's the shape's texture pointer.
		\return Returns a const pointer to the shape's texture.
	*/
	const Texture2D *GetTexture() const {
		return m_Texture;
	}
	/*!
		\brief Sets the shape's texture.
		\param p_Texture A pointer to the shape's new texture.
		\return Nothing.
	*/
	void SetTexture(Texture2D *p_Texture) {
		m_Texture = p_Texture;
	}

	/*!
		\brief Gets the shape's colour,
		\return Returns the shape's colour.
	*/
	const glm::vec4 &GetColour() const {
		return m_Colour;
	}
	/*!
		\brief Sets the shape's colour.
		\param p_Colour The shape's new colour.
		\return Nothing.
	*/
	void SetColour(const glm::vec4 &p_Colour) {
		m_Colour = p_Colour;
	}

	/*!
		\brief Gets the shape's colour texture mix value.
		\return Returns the shape's colour texture mix value.
	*/
	float GetColourTextureMixValue() const {
		return m_ColourTextureMixValue;
	}
	/*!
		\brief Sets the shape's colour texture mix value.
		\param p_ColourTextureMixValue The shape's new colour texture mix value.
		\return Nothing.
	*/
	void SetColourTextureMixValue(float p_ColourTextureMixValue) {
		m_ColourTextureMixValue = p_ColourTextureMixValue;
	}

	/*!
		\brief Gets whether the colour is a mask for the texture.
		\return Returns true if the colour is being used as a mask for the texture.
	*/
	bool GetUsingColourAsTextureMask() const {
		return m_UsingColourAsTextureMask;
	}
	/*!
		\brief Sets whether the shape is using the colour as a texture mask.
		\param p_UsingColourAsTextureMask True if the colour is being used as a texture mask, false otherwise.
		\return Nothing.
	*/
	void SetUsingColourAsTextureMask(bool p_UsingColourAsTextureMask) {
		m_UsingColourAsTextureMask = p_UsingColourAsTextureMask;
	}
};