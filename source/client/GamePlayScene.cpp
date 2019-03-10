#include "GamePlayScene.h"

#include <SFML/Window/Event.hpp>

#include "EntityManager.h"
#include "ParticleManager.h"
#include "Logger.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

GamePlayScene::GamePlayScene(const Vector2D<float> &p_ScreenSize, const std::string &p_File) 
	: Scene(p_ScreenSize, p_File) {
	m_ParticleManager = std::make_shared<ParticleManager>(p_ScreenSize, MAX_NUMBER_OF_PARTICLES);
	
	m_Client.Connect(sf::Time::Zero);
}

GamePlayScene::~GamePlayScene() {
	// Disconnect from the server, gracefully.
	if(m_Client.IsConnected())
		m_Client.Disconnect();
}

void GamePlayScene::HandleInputEvent(sf::Event &p_Event) {
	m_MousePosition = Vector2Df((float)p_Event.mouseMove.x, (float)p_Event.mouseMove.y);

	if (p_Event.type == sf::Event::MouseButtonPressed) {
		Vector2D<float> mousePosition((float)p_Event.mouseMove.x, (float)p_Event.mouseMove.y);

		for (auto &button : m_UserInterface->GetButtons()) {
			if (button->m_ShapeType == typeid(RectangleShape)) {
				std::shared_ptr<RectangleShape> shape = std::static_pointer_cast<RectangleShape>(button->m_Shape);
				if (m_Collision(shape->GetPosition(), shape->GetSize(), mousePosition)) {
					if (p_Event.type == p_Event.MouseButtonPressed)
						m_GameState = button->m_GameState;
				}
			}
			else {
				std::shared_ptr<CircleShape> shape = std::static_pointer_cast<CircleShape>(button->m_Shape);
				if (m_Collision(shape->GetPosition(), shape->GetRadius(), mousePosition)) {
					if (p_Event.type == p_Event.MouseButtonPressed)
						m_GameState = button->m_GameState;
				}
			}
		}

		std::shared_ptr<TransformComponent> potentialTransformComponent = EntityManagerInstance.GetComponent<TransformComponent>(m_PlayerEntityID);
		if (potentialTransformComponent != nullptr) {
			TransformComponent &transformComponent = *potentialTransformComponent;
			for (int i = 1; i < transformComponent.m_CircleTransforms.size(); i++) {
				CircleTransformData &circleTransformData = transformComponent.m_CircleTransforms[i];
				if (m_Collision(circleTransformData.m_Position, circleTransformData.m_Radius, mousePosition)) {
					// If the player has clicked on a symbol get the symbol ID, from the render component.
					m_PlayerSymbolIDGuess = EntityManagerInstance.GetComponent<RenderComponent>(m_PlayerEntityID)->m_SymbolTextureIDs[i];
					Log(Type::INFO) << "The player has clicked on a symbol. Symbol ID: " << m_PlayerSymbolIDGuess << "\nEntity Name: " << m_PlayerEntityID;
					
					if (m_UserInterface->Time() - m_TimeOfLastAttempt >= ATTEMPT_DELAY) {
						Log(Type::INFO) << "Symbol sent to the server!" << m_PlayerEntityID;
						// The player has attempted to guess the symbol, send it to the server.
						sf::Packet packet = Packet::SetPacketType(Packet::SYMBOL_ID);
						packet << m_PlayerSymbolIDGuess;
						m_Client.Send(packet);

						m_TimeOfLastAttempt = m_UserInterface->Time();
					}
					m_PlayerSymbolIDGuess = INVALID_SYMBOL_GUESS;
				}
			}
		}
		Log(Type::INFO) << "Mouse X Position: " << mousePosition.X() << "\tMouse Y Position: " << mousePosition.Y();
	}
}

void GamePlayScene::Update(float p_DeltaTime) {
	EntityManagerInstance.UpdateSystems(p_DeltaTime);
	m_UserInterface->Update(p_DeltaTime);
	m_PostProcessor->Update(p_DeltaTime);

	static float timePassed;
	timePassed += p_DeltaTime;
	if (timePassed >= 10.0f) {
		m_PostProcessor->SetShakeState(true);
		m_PostProcessor->SetShakeTime(4.5f);
		timePassed = 0.0f;
	}

	m_ParticleManager->Update(p_DeltaTime, m_MousePosition);

	sf::Packet packet;
	if (m_Client.ReceiveData(packet)) {
		HandlePacket(packet);
	}
}

void GamePlayScene::Render(Window &p_Window) {
	// Start rendering to the post processing quad.
	m_PostProcessor->BeginRender();
	m_UserInterface->Render();

	EntityManagerInstance.RenderSystems(p_Window);

	m_ParticleManager->Render();

	// End rendering to the post processing quad.
	m_PostProcessor->EndRender();
	m_PostProcessor->Render();

	RenderText("Score: " + std::to_string(static_cast<int>(m_Score)), Vector2D<float>(0.01f, 0.955f), 0.55f, glm::vec3(0.2f, 0.5f, 0.1f));
	RenderText("Time: " + std::to_string(static_cast<int>(m_UserInterface->Time())), Vector2D<float>(0.88f, 0.955f), 0.55f, glm::vec3(0.2f, 0.5f, 0.1f));
}

void GamePlayScene::HandlePacket(sf::Packet &p_Packet) {
	PacketID packetID = Packet::GetPacketType(p_Packet);

	float quarterWidth = (float)m_ScreenSize.X() / 4.0f;
	float heightOffset = (float)m_ScreenSize.Y() / 2.0f;
	
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

		m_TimeOfLastAttempt = m_UserInterface->Time() - ATTEMPT_DELAY;
	}
	else if (packetID == Packet::SCORE) {
		p_Packet >> m_Score;
		Log(Type::INFO) << "Score: " << m_Score;
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
		m_GameState = GameState::MAIN_MENU;
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