#include "GamePlayScene.h"

#include "EntityManager.h"
#include "Logger.h"

#include "RenderComponent.h"

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

	// TEMP:
	int width = 1280;
	float quarterWidth = (float)width / 4.0f;
	int height = 720;
	float heightOffset = (float)height / 2.5f;

	if (packetID == Packet::PLAYER_CARD_DATA) {
		CreateCardEntity("DeckCard", p_Packet, Vector2D<float>(quarterWidth, heightOffset));
	}
	else if (packetID == Packet::DECK_CARD_DATA) {
		CreateCardEntity("MyCard", p_Packet, Vector2D<float>(quarterWidth * 3, heightOffset));
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

void GamePlayScene::CreateCardEntity(const std::string &p_EntityName, sf::Packet &p_Packet, const Vector2D<float> &p_PositionOffset) {
	EntityManagerInstance.DeleteComponent<RenderComponent>(p_EntityName);
	EntityManagerInstance.DeleteComponent<TransformComponent>(p_EntityName);
	EntityManagerInstance.CreateEntity(p_EntityName);

	// Render component:
	RenderComponent renderComponent;
	p_Packet >> renderComponent;
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(p_EntityName, std::make_shared<RenderComponent>(renderComponent));

	// Transform component:
	TransformComponent transformComponent;
	p_Packet >> transformComponent;
	for (auto &tranform : transformComponent.m_CircleTransforms)
		tranform.m_Position += p_PositionOffset;
	EntityManagerInstance.AddComponentToEntity<TransformComponent>(p_EntityName, std::make_shared<TransformComponent>(transformComponent));
}