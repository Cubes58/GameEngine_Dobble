#pragma once

#include "Scene.h"

#include <string>

#include "Client.h"
#include "Vector2D.h"

#define INVALID_SYMBOL_GUESS -1
#define ATTEMPT_DELAY 4.0f
#define MAX_NUMBER_OF_PARTICLES 500

class ParticleManager;

class GamePlayScene : public Scene {
private:
	Client m_Client;
	std::string m_PlayerEntityID = "PlayerCard";
	std::string m_DeckEntityID = "DeckCard";
	Vector2Df m_MousePosition;
	sf::Int32 m_PlayerSymbolIDGuess = INVALID_SYMBOL_GUESS;
	sf::Int32 m_RoundsWon = 0;
	float m_Score = 0.0f;
	float m_TimeOfLastAttempt = -ATTEMPT_DELAY;
	std::shared_ptr<ParticleManager> m_ParticleManager;

	void HandlePacket(sf::Packet &p_Packet);
	void CreateCardEntity(const std::string &p_EntityName, sf::Packet &p_Packet, const Vector2D<float> &p_PositionOffset = Vector2D<float>(0.0f, 0.0f));

public:
	GamePlayScene(const Vector2D<float> &p_ScreenSize, const std::string &p_File);
	~GamePlayScene();

	virtual void HandleInputEvent(sf::Event &p_Event) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render(Window &p_Window) override;
};