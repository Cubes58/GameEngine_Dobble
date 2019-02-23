#pragma once

#include <vector>

#include "Component.h"
#include "PacketTypes.h"

struct RenderComponent : public Component {
	std::vector<unsigned int> m_SymbolTextureIDs;

	RenderComponent() = default;
	RenderComponent(unsigned int p_NumberOfCirclesPerCard) {
		m_SymbolTextureIDs.resize(p_NumberOfCirclesPerCard);
	}
	RenderComponent(const std::vector<unsigned int> &p_SymbolNamesIDs) : m_SymbolTextureIDs(p_SymbolNamesIDs) {}
};

sf::Packet &operator<<(sf::Packet &p_Packet, RenderComponent &p_RenderComponent);
sf::Packet &operator>>(sf::Packet &p_Packet, RenderComponent &p_RenderComponent);