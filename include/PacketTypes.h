/**
@file PacketTypes.h
@brief The different packet types available and the ability to get and set their type.
*/
#pragma once

#include <SFML/Network/Packet.hpp>

using PacketID = sf::Int8;	//!< First 8 bits are the PacketID, using PacketID as an sf::Int8.

/**
@file PacketType.h
@enum PacketType
@brief The different packet types available.
*/

namespace Packet {
	enum PacketType {
		DISCONNECT = -1,	/**< Disconnect from the server. */
		CONNECT = 0,		/**< Connection established. */
		STARTING_GAME,		/**< When the game is starting. */
		SYMBOL_ID,			/**< Players' symbol guess. */
		PLAYER_CARD_DATA,	/**< Send the information for the player's card. */
		DECK_CARD_DATA,		/**< Send the information for the deck's top card. */
		ROUND_FINISHED,		/**< Send the winner, of the round. */
		SCORE,				/**< Send the player's score. */
		GAME_FINISHED,		/**< Send the winner, of the game. */

		NOT_AVAILABLE		/**< Unknown packet/ignore it. */
	};

	/*!
		\brief Gets the packet type.
		\param p_Packet The packet you want the type of.
		\return Returns the packet's type.
	*/
	PacketID GetPacketType(sf::Packet &p_Packet);

	/*!
		\brief Sets the packet type.
		\param p_Type The type to set the packet as.
		\param p_Packet The packet to set the type to.
		\return Nothing.
	*/
	void SetPacketType(const PacketType &p_Type, sf::Packet &p_Packet);

	/*!
		\brief Sets the packet type.
		\param p_Type The type to set the packet as.
		\return Returns the set packet.
	*/
	sf::Packet SetPacketType(const PacketType &p_Type = Packet::NOT_AVAILABLE);
}