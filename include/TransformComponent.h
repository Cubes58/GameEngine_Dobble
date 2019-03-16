/**
@file TransformComponent.h
@brief A class that stores the properties necessary for a transform component, to be added to an entity.
*/
#pragma once

#include <vector>

#include "Component.h"
#include "Vector2D.h"
#include "PacketTypes.h"

/**
	* A structure to represent circle transform data.
*/

struct CircleTransformData {
	Vector2Df m_Position;	//!< The position.
	float m_Radius;			//!< The radius.
	float m_Rotation;		//!< The rotation.

	/*!
		\brief Constructor
	*/
	CircleTransformData() : m_Position(0.0f, 0.0f), m_Radius(0.0f), m_Rotation(0.0f) {}
	/*!
		\brief Constructor
		\param p_Position the position of the circle.
	*/
	CircleTransformData(Vector2Df p_Position) : m_Position(p_Position), m_Radius(0.0f), m_Rotation(0.0f) {}
	/*!
		\brief Constructor
		\param p_Position the position of the circle.
		\param p_Radius the circle's radius.
	*/
	CircleTransformData(Vector2Df p_Position, float p_Radius) : m_Position(p_Position), m_Radius(p_Radius), m_Rotation(0.0f) {}
	/*!
		\brief Constructor
		\param p_Position the position of the circle.
		\param p_Radius the circle's radius.
		\param p_Rotation the circle's rotation.
	*/
	CircleTransformData(Vector2Df p_Position, float p_Radius, float p_Rotation) : m_Position(p_Position), m_Radius(p_Radius), m_Rotation(p_Rotation) {}

	/*!
		@ref CircleTransform::operator==(CircleTransformData &)
	*/
	bool operator==(const CircleTransformData &p_CircleTransformData) {
		return (this->m_Position == p_CircleTransformData.m_Position 
			&& this->m_Radius == p_CircleTransformData.m_Radius 
			&& this->m_Rotation == p_CircleTransformData.m_Rotation);
	}
	/*!
		@ref CircleTransform::operator!=(CircleTransformData &)
	*/
	bool operator!=(const CircleTransformData &p_CircleTransformData) {
		return !(*this == p_CircleTransformData);
	}
};

/**
	* A structure to represent a transform component.
*/
struct TransformComponent : public Component {
	std::vector<CircleTransformData> m_CircleTransforms;	//!< The circle transform data.

	TransformComponent() = default;	//!< Constructor.
	/*!
		\brief Constructor
		\param p_NumberOfCirclesPerCard the number of circle each card will have (includes the card's background circle).
	*/
	TransformComponent(unsigned int p_NumberOfCirclesPerCard) {
		m_CircleTransforms.reserve(p_NumberOfCirclesPerCard);
		for (unsigned int i = 0; i < p_NumberOfCirclesPerCard; i++) {
			m_CircleTransforms.emplace_back(CircleTransformData());
		}
	}
	/*!
		\brief Constructor
		\param p_CirclePositions a vector of circle positions, for each circle on the card (including the card's background circle).
	*/
	TransformComponent(const std::vector<CircleTransformData> &p_CirclePositions) 
		: m_CircleTransforms(p_CirclePositions) {}
};

/*!
	@ref sf::Packet::operator<<(sf::Packet &, TransformComponent &)
*/
sf::Packet &operator<<(sf::Packet &p_Packet, TransformComponent &p_TransformComponent);

/*!
	@ref sf::Packet::operator>>(sf::Packet &, TransformComponent &)
*/
sf::Packet &operator>>(sf::Packet &p_Packet, TransformComponent &p_TransformComponent);