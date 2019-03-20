#include "GamePlayScene.h"

#include <SFML/Window/Event.hpp>

#include "EntityManager.h"
#include "RenderSystem.h"
#include "AudioManager.h"
#include "ParticleManager.h"
#include "UserInterface.h"
#include "FontRenderer.h"
#include "PostProcessor.h"
#include "Randomiser.h"
#include "Logger.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

GamePlayScene::GamePlayScene(const Vector2Df &p_ScreenSize, const std::string &p_File) 
	: Scene(p_ScreenSize, p_File) {
	EntityManagerInstance.Init();
	EntityManagerInstance.AddSystem(std::make_shared<RenderSystem>((float)p_ScreenSize.X(), (float)p_ScreenSize.Y()));

	m_ParticleManager = std::make_shared<ParticleManager>(p_ScreenSize, MAX_NUMBER_OF_PARTICLES);

	m_CouldConnect = m_Client.Connect(sf::Time::Zero);
}

GamePlayScene::~GamePlayScene() {
	// Disconnect from the server, gracefully.
	m_Client.Disconnect();
	EntityManagerInstance.Clear();
}

void GamePlayScene::HandleInputEvent(sf::Event &p_Event) {
	m_MousePosition = Vector2Df((float)p_Event.mouseMove.x, (float)p_Event.mouseMove.y);

	m_GameState = m_UserInterface->HandleInput(p_Event, m_GameState);
	if (p_Event.type == sf::Event::MouseButtonPressed) {
		std::shared_ptr<TransformComponent> potentialTransformComponent = EntityManagerInstance.GetComponent<TransformComponent>(m_PlayerEntityID);
		if (potentialTransformComponent != nullptr) {
			TransformComponent &transformComponent = *potentialTransformComponent;
			for (int i = 1; i < transformComponent.m_CircleTransforms.size(); i++) {
				CircleTransformData &circleTransformData = transformComponent.m_CircleTransforms[i];
				if (m_Collision(circleTransformData.m_Position, circleTransformData.m_Radius, m_MousePosition)) {
					// If the player has clicked on a symbol get the symbol ID, from the render component.
					m_PlayerSymbolIDGuess = EntityManagerInstance.GetComponent<RenderComponent>(m_PlayerEntityID)->m_SymbolTextureIDs[i];
					Log(Type::INFO) << "The player has clicked on a symbol. Symbol ID: " << m_PlayerSymbolIDGuess << "\nEntity Name: " << m_PlayerEntityID;
					
					// Check whether a certain amount of time has passed, since the previous guess, to stop the player spamming guesses.
					if (m_UserInterface->Time() - m_TimeOfLastAttempt >= ATTEMPT_DELAY) {
						Log(Type::INFO) << "Symbol sent to the server!" << m_PlayerEntityID;
						AudioManagerInstance.PlaySoundEffect("symbolGuess");
						// The player has attempted to guess the symbol, send it to the server.
						sf::Packet packet = Packet::SetPacketType(Packet::SYMBOL_ID);
						packet << m_PlayerSymbolIDGuess;
						m_Client.Send(packet);

						m_TimeOfLastAttempt = m_UserInterface->Time();
					}
					else 
						AudioManagerInstance.PlaySoundEffect("squat");
					m_PlayerSymbolIDGuess = INVALID_SYMBOL_GUESS;
				}
			}
		}
	}
}

void GamePlayScene::Update(float p_DeltaTime) {
	if (!m_CouldConnect) {
		m_GameState = GameState::MAIN_MENU;
		return;
	}

	m_TimePassedSinceShakeEffectActivated += p_DeltaTime;
	m_TimePassedSinceOtherEffectActivated += p_DeltaTime;

	EntityManagerInstance.UpdateSystems(p_DeltaTime);
	m_UserInterface->Update(p_DeltaTime);
	m_PostProcessor->Update(p_DeltaTime);
	m_ParticleManager->Update(p_DeltaTime, m_MousePosition);
	
	if (m_TimePassedSinceShakeEffectActivated >= m_ShakeEffectActivationGap) {
		m_ShakeEffectActivationGap = RandomiserInstance.GetUniformRealRandomNumber(MIN_GAP_DURATION_BETWEEN_SHAKE_EFFECT, MAX_GAP_DURATION_BETWEEN_SHAKE_EFFECT);
		m_PostProcessor->SetShakeState(true);
		m_PostProcessor->SetShakeTime(RandomiserInstance.GetUniformRealRandomNumber(MIN_SHAKE_TIME_EFFECT_DURATION, MAX_SHAKE_TIME_EFFECT_DURATION));
		m_TimePassedSinceShakeEffectActivated = 0.0f;
	}
	if (m_TimePassedSinceOtherEffectActivated >= m_OtherEffectActivationGap) {
		m_OtherEffectActivationGap = RandomiserInstance.GetUniformRealRandomNumber(MIN_GAP_DURATION_BETWEEN_OTHER_EFFECT, MAX_GAP_DURATION_BETWEEN_OTHER_EFFECT);
		// Randomly generate a number, if it's an even number set the inverted colours state, otherwise set the chaos state.
		RandomiserInstance.GetUniformIntegerRandomNumber(1, 100) % 2 == 0 ? m_PostProcessor->SetInvertColoursState(true) : m_PostProcessor->SetChaosState(true);
		m_PostProcessor->SetOtherEffectTime(RandomiserInstance.GetUniformRealRandomNumber(MIN_OTHER_EFFECT_TIME_DURATION, MAX_OTHER_EFFECT_TIME_DURATION));
		m_TimePassedSinceOtherEffectActivated = 0.0f;
	}

	sf::Packet packet;
	if (m_Client.ReceiveData(packet)) {
		HandlePacket(packet);
	}
}

void GamePlayScene::Render() {
	// Start rendering to the post processing quad.
	m_PostProcessor->BeginRender();
	m_UserInterface->Render();

	EntityManagerInstance.RenderSystems();
	m_ParticleManager->Render();

	// End rendering to the post processing quad.
	m_PostProcessor->EndRender();
	m_PostProcessor->Render();

	glm::vec3 playerScoreAndTimeColour = glm::vec3(0.2f, 0.5f, 0.1f);
	glm::vec3 enemyScoreAndTimeColour = glm::vec3(0.439, 0.098, 0.439);
	if (!(m_UserInterface->Time() - m_TimeOfLastAttempt >= ATTEMPT_DELAY)) {
		playerScoreAndTimeColour = glm::vec3(0.439, 0.098, 0.098);
		enemyScoreAndTimeColour = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	Vector2Df enemyScorePosition(0.15f, 0.955f);
	for (auto &enemyScore : m_EnemyScores) {
		RenderText("Enemy Score: " + std::to_string(static_cast<int>(enemyScore)), enemyScorePosition, 0.55f, enemyScoreAndTimeColour);
		enemyScorePosition.SetX(enemyScorePosition.X() + 0.22f);
	}

	RenderText("Score: " + std::to_string(static_cast<int>(m_Score)), Vector2Df(0.01f, 0.955f), 0.55f, playerScoreAndTimeColour);
	RenderText("Time: " + std::to_string(static_cast<int>(m_UserInterface->Time())), Vector2Df(0.88f, 0.955f), 0.55f, playerScoreAndTimeColour);
}

void GamePlayScene::ReadScore(sf::Packet &p_Packet) {
	float scoreData = 0.0f;
	bool isForThisClient = false;
	unsigned int enemyScoreID = 0;
	while ((p_Packet >> scoreData) && (p_Packet >> isForThisClient)) {
		if (isForThisClient) {
			m_Score = scoreData;
			Log(Type::INFO) << "Score: " << m_Score;
		}
		else {
			m_EnemyScores[enemyScoreID] = scoreData;
			Log(Type::INFO) << "Enemy Score: " << m_EnemyScores[enemyScoreID];

			++enemyScoreID;
		}
	}
}

void GamePlayScene::HandlePacket(sf::Packet &p_Packet) {
	PacketID packetID = Packet::GetPacketType(p_Packet);
	
	if (packetID == Packet::PLAYER_CARD_DATA) {
		CreateCardEntity(m_PlayerEntityID, p_Packet, Vector2Df((float)m_ScreenSize.X() / 4.0f, (float)m_ScreenSize.Y() / 2.0f));
	}
	else if (packetID == Packet::DECK_CARD_DATA) {
		CreateCardEntity(m_DeckEntityID, p_Packet, Vector2Df(((float)m_ScreenSize.X() / 4.0f) * 3, (float)m_ScreenSize.Y() / 2.0f));
	}
	else if (packetID == Packet::ROUND_FINISHED) {
		bool hasWonRound = false;
		p_Packet >> hasWonRound;
		if (hasWonRound)
			++m_RoundsWon;

		m_TimeOfLastAttempt = m_UserInterface->Time() - ATTEMPT_DELAY;
	}
	else if (packetID == Packet::SCORE) {
		ReadScore(p_Packet);
	}
	else if (packetID == Packet::GAME_FINISHED) {
		bool hasPlayerWonGame = false;
		p_Packet >> hasPlayerWonGame;
		Log(Type::INFO) << "Game over. Has the player won: " << hasPlayerWonGame;
		if (hasPlayerWonGame)
			m_GameState = GameState::WIN;
		else
			m_GameState = GameState::LOSE;
	}
	else if (packetID == Packet::STARTING_GAME) {
		std::size_t numberOfEnemyPlayers = 0;
		p_Packet >> numberOfEnemyPlayers;
		m_EnemyScores.resize(numberOfEnemyPlayers);

		m_UserInterface->ResetTime();
	}
	else if (packetID == Packet::DISCONNECT) {
		m_GameState = GameState::MAIN_MENU;
	}
}

void GamePlayScene::CreateCardEntity(const std::string &p_EntityName, sf::Packet &p_Packet, const Vector2Df &p_PositionOffset) {
	EntityManagerInstance.DeleteComponent<RenderComponent>(p_EntityName);
	EntityManagerInstance.DeleteComponent<TransformComponent>(p_EntityName);
	// Ensure the entity is created.
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

float GamePlayScene::GetPlayerScore() {
	return m_Score;
}

int GamePlayScene::GetRoundsWon() {
	return m_RoundsWon;
}