#include "ServerGame.h"

ServerGame::ServerGame() : m_IsRunning(true) {
	m_Deck.GenerateCards(8, 200.0f);

	// Create Cards.
	// Connect to the server.


}

ServerGame::~ServerGame() {

}

void ServerGame::Update(float p_DeltaTime) {



}

bool ServerGame::IsRunning() const {
	return m_IsRunning;
}