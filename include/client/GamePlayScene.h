#pragma once

#include "Scene.h"

#include <string>

#include "Client.h"
#include "Vector2D.h"

#define INVALID_SYMBOL_GUESS -1

class GamePlayScene : public Scene {
private:
	Client m_Client;
	std::string m_PlayerEntityID = "PlayerCard";
	std::string m_DeckEntityID = "DeckCard";
	Vector2D<float> m_ScreenSize;
	sf::Int32 m_PlayerSymbolIDGuess = INVALID_SYMBOL_GUESS;

	void HandlePacket(sf::Packet &p_Packet);
	void CreateCardEntity(const std::string &p_EntityName, sf::Packet &p_Packet, const Vector2D<float> &p_PositionOffset = Vector2D<float>(0.0f, 0.0f));

public:
	GamePlayScene(const Vector2D<float> &p_ScreenSize);
	~GamePlayScene() = default;

	virtual void HandleInputEvent(sf::Event &p_Event) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render(Window &p_Window) override;
};