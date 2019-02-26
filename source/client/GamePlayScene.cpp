#include "GamePlayScene.h"

#include <SFML/Window/Event.hpp>

#include "EntityManager.h"
#include "Logger.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

GamePlayScene::GamePlayScene(const Vector2D<float> &p_ScreenSize) : m_ScreenSize(p_ScreenSize) {
	// Wait as long as it takes, to connect to the server (Maybe add to a config file, and allow a user to specify).
	m_Client.Connect(sf::Time::Zero);
}

GamePlayScene::~GamePlayScene() {
	// Disconnect from the server, gracefully.
	m_Client.Disconnect();
}

void GamePlayScene::HandleInputEvent(sf::Event &p_Event) {
	if (p_Event.type == sf::Event::MouseButtonPressed) {
		Vector2D<float> mousePosition((float)p_Event.mouseButton.x, (float)p_Event.mouseButton.y);

		auto checkIfPlayerClickedSymbol = [&](const std::string &p_EntityName)->bool { 
			std::shared_ptr<TransformComponent> potentialTransformComponent = EntityManagerInstance.GetComponent<TransformComponent>(p_EntityName);
			if (potentialTransformComponent != nullptr) {
				TransformComponent &transformComponent = *potentialTransformComponent;
				for (int i = 1; i < transformComponent.m_CircleTransforms.size(); i++) {
					CircleTransformData &circleTransformData = transformComponent.m_CircleTransforms[i];
					if (m_Collision(circleTransformData.m_Position, circleTransformData.m_Radius, mousePosition)) {
						// If the player has clicked on a symbol get the symbol ID, from the render component.
						m_PlayerSymbolIDGuess = EntityManagerInstance.GetComponent<RenderComponent>(p_EntityName)->m_SymbolTextureIDs[i];

						Log(Type::INFO) << "The player has clicked on a symbol. Symbol ID: " << m_PlayerSymbolIDGuess << "\nEntity Name: " << p_EntityName;
						return true;
					}
				}
			}
			return false;
		};
		if (checkIfPlayerClickedSymbol(m_PlayerEntityID)) {
			// The player has attempted to guess the symbol, send it to the server.
			sf::Packet packet = Packet::SetPacketType(Packet::SYMBOL_ID);
			packet << m_PlayerSymbolIDGuess;
			m_Client.Send(packet);

			m_PlayerSymbolIDGuess = INVALID_SYMBOL_GUESS;
		}
		Log(Type::INFO) << "Mouse X Position: " << mousePosition.X() << "\tMouse Y Position: " << mousePosition.Y();
	}
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

	float quarterWidth = (float)m_ScreenSize.X() / 4.0f;
	float heightOffset = (float)m_ScreenSize.Y() / 2.5f;
	
	if (packetID == Packet::PLAYER_CARD_DATA) {
		CreateCardEntity(m_PlayerEntityID, p_Packet, Vector2D<float>(quarterWidth, heightOffset));
	}
	else if (packetID == Packet::DECK_CARD_DATA) {
		CreateCardEntity(m_DeckEntityID, p_Packet, Vector2D<float>(quarterWidth * 3, heightOffset));
	}
	else if (packetID == Packet::ROUND_FINISHED) {
		bool hasWonRound = false;
		p_Packet >> hasWonRound;
		if (hasWonRound)
			++m_RoundsWon;
	}
	else if (packetID == Packet::SCORE) {
		p_Packet >> m_Score;
		Log(Type::INFO) << "Score: " << m_Score;
	}
	else if (packetID == Packet::SYMBOL_ID) {
		// For the client, this could be sent from the server, to inform the player which symbol was the correct guess, for them.
	}
	else if (packetID == Packet::GAME_FINISHED) {
		bool hasPlayerWonGame = false;
		p_Packet >> hasPlayerWonGame;
		Log(Type::INFO) << "Game over. Has player won: " << hasPlayerWonGame;
		if (hasPlayerWonGame)
			m_GameState = GameState::WIN;
		else
			m_GameState = GameState::LOSE;
	}
	else if (packetID == Packet::DISCONNECT) {
		// Disconnect from the Server.
		m_Client.Disconnect();
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