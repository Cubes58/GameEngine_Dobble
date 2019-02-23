#pragma once

#include "Scene.h"

#include <string>

#include "Client.h"
#include "Vector2D.h"

class GamePlayScene : public Scene {
private:
	Client m_Client;
	GameState m_GameState;

	void HandlePacket(sf::Packet &p_Packet);
	void CreateCardEntity(const std::string &p_EntityName, sf::Packet &p_Packet, const Vector2D<float> &p_PositionOffset = Vector2D<float>(0.0f, 0.0f));

public:
	GamePlayScene();
	~GamePlayScene() = default;

	virtual bool Change(GameState &p_GameState) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render(Window &p_Window) override;
};