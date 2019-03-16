/**
@file Text.h
@brief Stores the properties of text.
*/
#pragma once

#include <string>

#include <glm/vec3.hpp>

#include "Vector2D.h"

/**
	* A structure to represent data of a Text object.
*/

struct Text {
	std::string m_Text;	//!< The text to render.
	glm::vec3 m_Colour;	//!< The colour of the text.
	Vector2Df m_Position;	//!< The position of the text (0 to 1 range).
	float m_Size;	//!< The size of the text.

	/*!
		\brief Constructor.
	*/
	Text() = default;
	/*!
		\brief Constructor.
		\param p_Text The text to render.
		\param p_Position The position of the text.
		\param p_Size The size of the text.
		\param p_Colour The colour of the text.
	*/
	Text(const std::string &p_Text, const Vector2Df &p_Position, float p_Size, const glm::vec3 &p_Colour)
		: m_Text(p_Text), m_Position(p_Position), m_Size(p_Size), m_Colour(p_Colour) {  }
	/*!
		\brief Destructor.
	*/
	~Text() = default;

	/*!
		\brief Virtual method, derived classes can use to update the information being displayed.
	*/
	virtual void Update() {
		return;
	}
};

/**
	* A structure to represent data of a Text object, tied to another object.
*/
template<typename Type>
struct ReferenceText : Text {
	Type &m_ReferenceData;	//!< The object tied to the text.
	std::string m_OriginalText;	//!< The original string.

	/*!
		\brief Constructor.
		\param p_Text The text to render.
		\param p_ReferenceData The object (data) to add to the text.
		\param p_Position The position of the text.
		\param p_Size The size of the text.
		\param p_Colour The colour of the text.
	*/
	ReferenceText(const std::string &p_Text, Type &p_ReferenceData, const Vector2Df &p_Position, float p_Size, const glm::vec3 &p_Colour)
		: Text(p_Text, p_Position, p_Size, p_Colour), m_ReferenceData(p_ReferenceData), m_OriginalText(p_Text) {
	}
	/*!
		\brief Destructor.
	*/
	~ReferenceText() = default;

	/*!
		\brief Updates the information being displayed.
	*/
	virtual void Update() {
		m_Text = m_OriginalText + std::to_string((int)m_ReferenceData);
	}
};