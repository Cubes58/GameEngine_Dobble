#pragma once

#include <SFML/Network/Packet.hpp>

using PacketID = sf::Int8;	//!< First 8 bits are the PacketID.

namespace Packet {
	enum PacketType {
		DISCONNECT = -1,	// Disconnect from the server.
		CONNECT = 0,		// Connection established.
		INPUT,				// Take players' move.
		CARD_DATA,			// Send the information for a card.
		GAMEFINISHED		// Send the winner.
	};

	// Set the packet type.
	void SetPacketType(const PacketType &p_Type, sf::Packet &p_Packet);
	// Get the packet type.
	PacketID GetPacketType(sf::Packet &p_Packet);
}