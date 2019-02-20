#include "ServerGame.h"

#include "Logger.h"

ServerGame::ServerGame() : m_IsRunning(true) {
	// Connect to the server.
	m_Server.WaitForClientsToConnect(s_m_NumberOfPlayers);

	// Create Cards. (Need to get the num,ber of cards from the clients.) This generates cards, at origin, with a radius of 200.
	m_Deck.GenerateCards(Vector2D<float>(0.0f, 0.0f), 200.0f, 8);
}

ServerGame::~ServerGame() {

}

void ServerGame::SendStartingInformation() {
	// For every client connect, send starting information, such as: Their starting card, the card to match the symbol with (top of the deck).


	for (auto &client : m_Server.GetClientIDs()) {

	}
}

void ServerGame::Update(float p_DeltaTime) {

}

bool ServerGame::IsRunning() const {
	return m_IsRunning;
}