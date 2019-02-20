#pragma once

#include <SFML/Network/Packet.hpp>

using PacketID = sf::Int8;	//!< First 8 bits are the PacketID.

namespace Packet {
	enum PacketType {
		DISCONNECT = -1,	// Disconnect from the server.
		CONNECT = 0,		// Connection established.
		SYMBOL_ID,			// Players' symbol guess.
		CARD_DATA,			// Send the information for a card.
		GAME_FINISHED,		// Send the winner.

		NOT_AVAILABLE		// Unknown packet/ignore it.
	};

	// Get the packet type.
	PacketID GetPacketType(sf::Packet &p_Packet);
	// Set the packet type.
	void SetPacketType(const PacketType &p_Type, sf::Packet &p_Packet);
	sf::Packet SetPacketType(const PacketType &p_Type = Packet::NOT_AVAILABLE);
}