/**
@file Particle.h
@brief Contains the particle's information.
*/
#pragma once

#include "Vector2D.h"

#include <glm/vec4.hpp>

/**
	* A structure which contains all of the properties required to create a particle.
*/

struct Particle {
	glm::vec4 m_Colour;	//!< The colour of the particle.
	Vector2Df m_Position;	//!< The position of the particle.
	Vector2Df m_Velocity;	//!< The velocity of the particle.
	float m_Orientation;	//!< The orientation of the particle.
	float m_Life;	//!< How long the particle will live for.

	/*!
		\brief Constructor.
	*/
	Particle() : m_Position(0.0f), m_Velocity(0.0f), m_Colour(1.0f), m_Life(0.0f), m_Orientation(0.0f) {  }
};