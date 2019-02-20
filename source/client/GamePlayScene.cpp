#include "GamePlayScene.h"

#include "EntityManager.h"
#include "Logger.h"

GamePlayScene::GamePlayScene() {
	// Wait as long as it takes, to connect to the server (Maybe add to a config file, and allow a user to specify).
	m_Client.Connect(sf::Time::Zero);
}

bool GamePlayScene::Change(GameState &p_GameState) {
	if (static_cast<unsigned int>(m_GameState) != static_cast<unsigned int>(p_GameState)) {
		p_GameState = m_GameState;
		return true;
	}

	return false;
}

void GamePlayScene::Update(float p_DeltaTime) {
	EntityManagerInstance.UpdateSystems(p_DeltaTime);

	sf::Packet packet;
	if (m_Client.ReceiveData(packet)) {
		HandlePacket(packet);
	}
}

void GamePlayScene::Render(Window &p_Window) {
	EntityManagerInstance.RenderSystems(p_Window);

	// Also render the user interface information (Score/Time/How many cards left in the deck/Other players score/etc..)
}

void GamePlayScene::HandlePacket(sf::Packet &p_Packet) {
	PacketID packetID = Packet::GetPacketType(p_Packet);

	if (packetID == Packet::CARD_DATA) {
		// Get the information of the card.
	}
	else if (packetID == Packet::SYMBOL_ID) {
		// For the client, this could be sent from the server, to inform the player which symbol was the correct guess, for them.
	}
	else if (packetID == Packet::GAME_FINISHED) {
		// Game is over, check who won. (Might be this player, or the enemy.)
	}
	else if (packetID == Packet::DISCONNECT) {
		// Disconnect from the Server.
	}
	else if (packetID == Packet::CONNECT) {
		// Reconnect to the server.
	}
}