#include "PacketTypes.h"

namespace Packet {
	// Get the packet type.
	PacketID GetPacketType(sf::Packet &p_Packet) {
		PacketID packetID;
		p_Packet >> packetID;

		return packetID;
	}

	// Set the packet type.
	void SetPacketType(const PacketType &p_Type, sf::Packet &p_Packet) {
		p_Packet << PacketID(p_Type);
	}

	sf::Packet SetPacketType(const PacketType &p_Type) {
		sf::Packet packet;
		packet << PacketID(p_Type);

		return packet;
	}
}