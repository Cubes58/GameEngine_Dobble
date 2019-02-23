#include "RenderComponent.h"

#include "Logger.h"

sf::Packet &operator<<(sf::Packet &p_Packet, RenderComponent &p_RenderComponent) {
	p_Packet << sf::Int32(p_RenderComponent.m_SymbolTextureIDs.size());
	Log(MessageType::INFO) << "Number of circle in the render component: " << p_RenderComponent.m_SymbolTextureIDs.size();
	for (auto &symbol : p_RenderComponent.m_SymbolTextureIDs) {
		p_Packet << sf::Int32(symbol);
		Log(MessageType::INFO) << "Symbol ID: " << symbol;
	}

	return p_Packet;
}

sf::Packet &operator>>(sf::Packet &p_Packet, RenderComponent &p_RenderComponent) {
	sf::Int32 numberOfCircles = 0;
	p_Packet >> numberOfCircles;
	for (int i = 0; i < numberOfCircles; i++) {
		sf::Int32 symbolID;
		p_Packet >> symbolID;
		p_RenderComponent.m_SymbolTextureIDs.emplace_back(symbolID);
	}

	return p_Packet;
}