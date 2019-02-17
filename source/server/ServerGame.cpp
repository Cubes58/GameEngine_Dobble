#include "ServerGame.h"

#include "Logger.h"

ServerGame::ServerGame() : m_IsRunning(true) {
	// Connect to the server.
	// Create Cards.
	// Get number of symbols and card radius from the client.
	m_Deck.GenerateCards(Vector2D<float>(0.0f, 0.0f), 200.0f, 8);
}

ServerGame::~ServerGame() {

}

void ServerGame::Update(float p_DeltaTime) {
	m_Server.WaitForClientsToConnect(4);


}

bool ServerGame::IsRunning() const {
	return m_IsRunning;
}