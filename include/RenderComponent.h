/**
@file RenderComponent.h
@brief A class that stores the properties necessary for a render component, to be added to an entity.
*/
#pragma once

#include <vector>

#include "Component.h"
#include "PacketTypes.h"

/**
	* A structure to represent a render component.
*/

struct RenderComponent : public Component {
	std::vector<unsigned int> m_SymbolTextureIDs;	//!< Stores the texture symbols' IDs.

	RenderComponent() = default;	//!< Constructor.
	/*!
		\brief Constructor
		\param p_NumberOfCirclesPerCard the number of circle each card will have (includes the card's background circle).
	*/
	RenderComponent(unsigned int p_NumberOfCirclesPerCard) {
		m_SymbolTextureIDs.resize(p_NumberOfCirclesPerCard);
	}
	/*!
		\brief Constructor
		\param p_SymbolTextureIDs A vector of texture IDs, for each symbol on the card. 
	*/
	RenderComponent(const std::vector<unsigned int> &p_SymbolTextureIDs) : m_SymbolTextureIDs(p_SymbolTextureIDs) {}
};

/*!
	@ref sf::Packet::operator<<(sf::Packet &, RenderComponent &)
*/
sf::Packet &operator<<(sf::Packet &p_Packet, RenderComponent &p_RenderComponent);

/*!
	@ref sf::Packet::operator>>(sf::Packet &, RenderComponent &)
*/
sf::Packet &operator>>(sf::Packet &p_Packet, RenderComponent &p_RenderComponent);