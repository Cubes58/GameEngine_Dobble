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
		EntityManagerInstance.DeleteComponent<RenderComponent>("DeckCard");		// IMPROVE!!! Look for a better way!
		EntityManagerInstance.DeleteComponent<TransformComponent>("DeckCard");	// IMPROVE!!! Look for a better way!
		EntityManagerInstance.CreateEntity("DeckCard");							// IMPROVE!!! Look for a better way!

		// Render component:
		RenderComponent renderComponent;
		renderComponent.m_SymbolTextureIDs.emplace_back(0);
		for (int i = 0; i < s_NUMBER_OF_CIRCLES_PER_CARD - 1; i++) {
			sf::Int32 symbolID;
			p_Packet >> symbolID;
			renderComponent.m_SymbolTextureIDs.emplace_back(symbolID);
		}
		EntityManagerInstance.AddComponentToEntity<RenderComponent>("DeckCard", std::make_shared<RenderComponent>(renderComponent));

		// Transform component:
		TransformComponent transformComponent;
		for (int i = 0; i < s_NUMBER_OF_CIRCLES_PER_CARD; i++) {
			float xPosition;
			p_Packet >> xPosition;

			float yPosition;
			p_Packet >> yPosition;
			Vector2D<float> position(xPosition + quarterWidth, yPosition + heightOffset);
			//Vector2D<float> position(xPosition, yPosition);

			float radius;
			p_Packet >> radius;

			float rotation;
			p_Packet >> rotation;

			Log(MessageType::INFO) << "XPos: " << xPosition << "\tYPos: " << yPosition << "\tRadius: " << radius << "\Rotation: " << rotation;
			transformComponent.m_CircleTransforms.emplace_back(position, radius, rotation);
		}
		EntityManagerInstance.AddComponentToEntity<TransformComponent>("DeckCard", std::make_shared<TransformComponent>(transformComponent));
	}
	else if (packetID == Packet::DECK_CARD_DATA) {
		EntityManagerInstance.DeleteComponent<RenderComponent>("MyCard");		// IMPROVE!!! Look for a better way!
		EntityManagerInstance.DeleteComponent<TransformComponent>("MyCard");	// IMPROVE!!! Look for a better way!
		EntityManagerInstance.CreateEntity("MyCard");							// IMPROVE!!! Look for a better way!

		// Render component:
		RenderComponent renderComponent;
		renderComponent.m_SymbolTextureIDs.emplace_back(0);
		for (int i = 0; i < s_NUMBER_OF_CIRCLES_PER_CARD - 1; i++) {
			sf::Int32 symbolID;
			p_Packet >> symbolID;
			renderComponent.m_SymbolTextureIDs.emplace_back(symbolID);
		}
		EntityManagerInstance.AddComponentToEntity<RenderComponent>("MyCard", std::make_shared<RenderComponent>(renderComponent));

		// Transform component:
		TransformComponent transformComponent;
		for (int i = 0; i < s_NUMBER_OF_CIRCLES_PER_CARD; i++) {
			float xPosition;
			p_Packet >> xPosition;

			float yPosition;
			p_Packet >> yPosition;
			Vector2D<float> position(xPosition + (quarterWidth * 3), yPosition + heightOffset);

			float radius;
			p_Packet >> radius;

			float rotation;
			p_Packet >> rotation;

			Log(MessageType::INFO) << "XPos: " << xPosition << "\tYPos: " << yPosition << "\tRadius: " << radius << "\Rotation: " << rotation;
			transformComponent.m_CircleTransforms.emplace_back(position, radius, rotation);
		}
		EntityManagerInstance.AddComponentToEntity<TransformComponent>("MyCard", std::make_shared<TransformComponent>(transformComponent));
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