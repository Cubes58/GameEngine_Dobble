#include "ServerGame.h"

#include <vector>
#include <climits>

#include "EntityManager.h"
#include "Component.h"
#include "Logger.h"

ServerGame::ServerGame() : m_IsRunning(true) {
	// Connect to the server.
	m_Server.WaitForClientsToConnect(s_m_NumberOfPlayers);	// Once two people have connected wait another 7.5 seconds for more players - with a max.

	// Create Cards. (Need to get the number of cards/circles from the clients (Connect packet).) This generates cards, at origin, with a radius of 200.
	m_Deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, (unsigned int)6);
	SendStartingInformation();
}

ServerGame::~ServerGame() {

}

void ServerGame::SendStartingInformation() {
	// For every client connected, send starting information, such as: Their starting card, the card to match the symbol with (top of the deck).
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

void ServerGame::HandlePackets(std::map<ClientID, sf::Packet> &p_Data) {
	std::unordered_map<ClientID, bool> hasPlayerWonRound;
	int numberOfPlayersThatWon = 0;
	bool roundWon = false;

	for (auto &clientData : p_Data) {
		PacketID packetType = Packet::GetPacketType(clientData.second);

		if (packetType == Packet::SYMBOL_ID) {
			if (CheckIfPlayerWonRound(clientData.first, clientData.second)) {
				hasPlayerWonRound.emplace(clientData.first, true);
				roundWon = true;
				++numberOfPlayersThatWon;
			}
		}
		else if (packetType == Packet::DISCONNECT) {
			// Remove the client, and send them a DISCONNECT packet. UNTESTED!
			m_Server.Disconnect(clientData.first);
			m_PlayerScores.erase(clientData.first);
			Log(Type::INFO) << "Disconnecting from a client. Client ID: " << clientData.first;
			continue;
		}
		else {
			Log(Type::WARNING) << "Unknown packet type: " << packetType;
		}
	}

	bool gameOver = false;
	if (roundWon) {										// Deck card will also be needed to carry on the game (+1).
		if (m_Deck.NumberOfRemainingCards() < numberOfPlayersThatWon + 1) {
			m_GameOver = true;
		}
		else {
			for (auto &client : m_Server.GetClientIDs()) {
				sf::Packet roundFinishedPacket = Packet::SetPacketType(Packet::ROUND_FINISHED);

				auto iter = hasPlayerWonRound.find(client);
				if (iter != hasPlayerWonRound.end()) {
					// Check whether the player has won.
					if (iter->second) {
						// Increase the player's score.
						auto playerScore = m_PlayerScores.find(iter->first);
						if (playerScore == m_PlayerScores.end()) {
							// If the player's score cannot be found then create a score to manage, for the player.
							m_PlayerScores.emplace(iter->first, 0.0f);
							playerScore = m_PlayerScores.find(iter->first);
						}

						// Add the player's score.
						playerScore->second = playerScore->second + SCORE_GAINED_PER_GUESS;
						Log(Type::INFO) << "Client ID: " << playerScore->first << "\t" << "Score: " << playerScore->second;

						// Tell the player they've won the round.
						roundFinishedPacket << iter->second;
						m_Server.Send(client, roundFinishedPacket);

						// Send the player their new card.
						if (!gameOver) {
							sf::Packet playerCardPacket = Packet::SetPacketType(Packet::PLAYER_CARD_DATA);
							unsigned int playerCard = m_Deck.GetCardIDFromTop();
							playerCardPacket << *EntityManagerInstance.GetComponent<RenderComponent>(playerCard);
							playerCardPacket << *EntityManagerInstance.GetComponent<TransformComponent>(playerCard);
							m_Server.Send(client, playerCardPacket);
						}
						continue;
					}
				}
				// Player didn't win.
				roundFinishedPacket << false;
				m_Server.Send(client, roundFinishedPacket);
			}
		}

		for (auto &playerScore : m_PlayerScores) {
			sf::Packet scorePacket = Packet::SetPacketType(Packet::SCORE);
			scorePacket << playerScore.second;
			m_Server.Send(playerScore.first, scorePacket);
		}

		if (!m_GameOver) {
			// Set the new deck card.
			sf::Packet deckCardPacket = Packet::SetPacketType(Packet::DECK_CARD_DATA);
			m_ActiveDeckCard = m_Deck.GetCardIDFromTop();
			deckCardPacket << *EntityManagerInstance.GetComponent<RenderComponent>(m_ActiveDeckCard);
			deckCardPacket << *EntityManagerInstance.GetComponent<TransformComponent>(m_ActiveDeckCard);
			m_Server.Send(deckCardPacket);
		}
		else {
			// Check to see who has the highest score, then send inform all player's if they've won or lost.
			ClientID clientWithHighestScore = UINT_MAX;
			float highestScore = 0.0f;
			for (auto &playerScore : m_PlayerScores) {
				if (playerScore.second >= highestScore) {
					clientWithHighestScore = playerScore.first;
					highestScore = playerScore.second;
				}
			}
			for (auto &client : m_Server.GetClientIDs()) {
				sf::Packet gameFinishedPacket = Packet::SetPacketType(Packet::GAME_FINISHED);
				bool hasClientWon = false;
				if (client == clientWithHighestScore)
					hasClientWon = true;
				gameFinishedPacket << hasClientWon;

				m_Server.Send(client, gameFinishedPacket);
			}
		}
	}
}

bool ServerGame::CheckIfPlayerWonRound(const ClientID &p_ClientID, sf::Packet &p_Packet) {
	sf::Int32 playerGuessSymbolID(UINT_MAX);
	p_Packet >> playerGuessSymbolID;

	// Check the player's guess.
	if (Deck::HasMatchingSymbol(EntityManagerInstance.GetComponent<RenderComponent>(m_ActiveDeckCard), playerGuessSymbolID))
		return true;

	return false;
}

void ServerGame::Update(float p_DeltaTime) {
	// Get any packets from the clients.
	std::map<ClientID, sf::Packet> incomingData;
	if (m_Server.GetReceivedData(incomingData))
		HandlePackets(incomingData);
}

bool ServerGame::IsRunning() const {
	return m_IsRunning;
}