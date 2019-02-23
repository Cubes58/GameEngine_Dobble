#include "ServerGame.h"

#include <climits>

#include "EntityManager.h"
#include "Component.h"
#include "Logger.h"

ServerGame::ServerGame() : m_IsRunning(true) {
	// Connect to the server.
	m_Server.WaitForClientsToConnect(s_m_NumberOfPlayers);

	// Create Cards. (Need to get the number of cards/circles from the clients (Connect packet).) This generates cards, at origin, with a radius of 200.
	m_Deck.GenerateCards(Vector2D<float>(0.0f, 0.0f), 200.0f, 8);
	SendStartingInformation();
}

ServerGame::~ServerGame() {

}

void ServerGame::SendStartingInformation() {
	// For every client connect, send starting information, such as: Their starting card, the card to match the symbol with (top of the deck).
	sf::Packet deckCardPacket = Packet::SetPacketType(Packet::DECK_CARD_DATA);
	m_ActiveDeckCard = m_Deck.GetCardIDFromTop(); 
	deckCardPacket << *EntityManagerInstance.GetComponent<RenderComponent>(m_ActiveDeckCard);
	deckCardPacket << *EntityManagerInstance.GetComponent<TransformComponent>(m_ActiveDeckCard);
	m_Server.Send(deckCardPacket);

	for (auto &client : m_Server.GetClientIDs()) {
		sf::Packet playerCardPacket = Packet::SetPacketType(Packet::PLAYER_CARD_DATA);
		unsigned int playerCard = m_Deck.GetCardIDFromTop();
		playerCardPacket << *EntityManagerInstance.GetComponent<RenderComponent>(playerCard);
		playerCardPacket << *EntityManagerInstance.GetComponent<TransformComponent>(playerCard);
		m_Server.Send(client, playerCardPacket);
	}
}

bool ServerGame::CheckIfPlayerWonRound(const ClientID &p_ClientID, sf::Packet &p_Packet) {
	PacketID packetID = Packet::GetPacketType(p_Packet);
	
	if (packetID == Packet::SYMBOL_ID) {
		unsigned int playerGuessSymbolID(UINT_MAX);
		p_Packet >> playerGuessSymbolID;

		// Check the player's guess.
		if (Deck::HasMatchingSymbol(EntityManagerInstance.GetComponent<RenderComponent>(m_ActiveDeckCard), playerGuessSymbolID)) {
			return true;
		}
	}
	else if (packetID == Packet::DISCONNECT) {
		// Maybe listen out for another player, or continue with the game, and remove this player.
	}
	else {
		Log(MessageType::FAULT) << "Unkown packet ID, received from client: " << p_ClientID;
	}

	return false;
}

void ServerGame::Update(float p_DeltaTime) {
	// Get any packets from the clients.
	std::map<ClientID, sf::Packet> data;
	if (m_Server.GetReceivedData(data)) {
		// Process any packets.
		std::unordered_map<ClientID, bool> hasPlayerWonRound;
		bool roundWon = false;
		for (auto &clientData : data) {
			if (CheckIfPlayerWonRound(clientData.first, clientData.second)) {
				hasPlayerWonRound.emplace(clientData.first, true);
				roundWon = true;
			}
		}
		// Check for a winner.
		if (roundWon) {
			for (auto &client : m_Server.GetClientIDs()) {
				sf::Packet roundFinishedPacket = Packet::SetPacketType(Packet::ROUND_FINISHED);

				auto iter = hasPlayerWonRound.find(client);
				if (iter != hasPlayerWonRound.end()) {
					// Check whether the player has won.
					if (iter->second) {
						// Increment the player's score. (ADD!)

						// Tell the player they've won the round.
						roundFinishedPacket << iter->second;
						m_Server.Send(client, roundFinishedPacket);

						// Send the player their new card.
						sf::Packet playerCardPacket = Packet::SetPacketType(Packet::PLAYER_CARD_DATA);
						unsigned int playerCard = m_Deck.GetCardIDFromTop();
						playerCardPacket << *EntityManagerInstance.GetComponent<RenderComponent>(playerCard);
						playerCardPacket << *EntityManagerInstance.GetComponent<TransformComponent>(playerCard);
						m_Server.Send(client, playerCardPacket);
						continue;
					}
				}
				// Player didn't win.
				roundFinishedPacket << false;
				m_Server.Send(client, roundFinishedPacket);
			}

			// Set the new deck card.
			sf::Packet deckCardPacket = Packet::SetPacketType(Packet::DECK_CARD_DATA);
			m_ActiveDeckCard = m_Deck.GetCardIDFromTop();
			deckCardPacket << *EntityManagerInstance.GetComponent<RenderComponent>(m_ActiveDeckCard);
			deckCardPacket << *EntityManagerInstance.GetComponent<TransformComponent>(m_ActiveDeckCard);
			m_Server.Send(deckCardPacket);

		}
	}

	// ERROR HERE :: ONCE THE LAST CARD HAS BEEN REMOVED FROM THE DECK THE GAME WILL BE CONSIDERED OVER, BUT THERES STILL THE FINAL CARD!!!
	// MAYBE ADD A CLASS BOOL, WHICH CHECKS WHETHER THE GAME IS ON THE FINAL CARD!
	if (m_Deck.IsDeckEmpty()) {
		// Check if a player has won the game (Who has the highest score?)
	}
}

bool ServerGame::IsRunning() const {
	return m_IsRunning;
}