#pragma once

#include "Scene.h"

#include "Client.h"

class GamePlayScene : public Scene {
private:
	Client m_Client;
	GameState m_GameState;

	void HandlePacket(sf::Packet &p_Packet);

public:
	GamePlayScene();
	~GamePlayScene() = default;

	virtual bool Change(GameState &p_GameState) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render(Window &p_Window) override;
};