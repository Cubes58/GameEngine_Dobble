#pragma once

#include <SFML/Network/Packet.hpp>

using PacketID = sf::Int8;	//!< First 8 bits are the PacketID.

namespace Packet {
	enum PacketType {
		DISCONNECT = -1,	// Disconnect from the server.
		CONNECT = 0,		// Connection established.
		SYMBOL_ID,			// Players' symbol guess.
		PLAYER_CARD_DATA,	// Send the information for the player's card.
		DECK_CARD_DATA,		// Send the information for the deck's top card.
		ROUND_FINISHED,		// Send the winner, of the round.
		SCORE,				// Send the player's score.
		GAME_FINISHED,		// Send the winner, of the game.

		NOT_AVAILABLE		// Unknown packet/ignore it.
	};

	// Get the packet type.
	PacketID GetPacketType(sf::Packet &p_Packet);
	// Set the packet type.
	void SetPacketType(const PacketType &p_Type, sf::Packet &p_Packet);
	sf::Packet SetPacketType(const PacketType &p_Type = Packet::NOT_AVAILABLE);
}