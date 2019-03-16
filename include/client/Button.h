/**
@file Button.h
@brief Manages the properties required to create a button.
*/
#pragma once

#include <typeindex>
#include <memory>

#include "Vector2D.h"
#include "GameState.h"

class Shape;

/**
	* A structure which manages the properties required to create a button.
*/

struct Button {
	std::shared_ptr<Shape> m_Shape;	//!< A pointer to the shape instance, used as the button. 
	std::type_index m_ShapeType;	//!< The type of shape stored.
	GameState m_GameState;	//!< The game state, which is activated when the button is clicked.

	/*!
		\brief Constructor.
	*/
	Button() = default;
	/*!
		\brief Constructor.
		\param p_Shape The shape of the button.
		\param p_ShapeType The type of the shape.
	*/
	Button(std::shared_ptr<Shape> p_Shape, const std::type_index &p_ShapeType) 
		: m_Shape(p_Shape), m_ShapeType(p_ShapeType) { }
	/*!
		\brief Constructor.
		\param p_Shape The shape of the button.
		\param p_ShapeType The type of the shape.
		\param p_GameState The state which the game switches to, when the button is pressed.
	*/
	Button(std::shared_ptr<Shape> p_Shape, const std::type_index &p_ShapeType, const GameState &p_GameState)
		: m_Shape(p_Shape), m_ShapeType(p_ShapeType), m_GameState(p_GameState) { }
	/*!
		\brief Destructor.
	*/
	~Button() = default;
};