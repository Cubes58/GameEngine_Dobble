#include "PacketTypes.h"

namespace Packet {
	// Set the packet type.
	void SetPacketType(const PacketType &p_Type, sf::Packet &p_Packet) {
		p_Packet << PacketID(p_Type);
	}

	// Get the packet type.
	PacketID GetPacketType(sf::Packet &p_Packet) {
		PacketID packetID;
		p_Packet >> packetID;

		return packetID;
	}
}