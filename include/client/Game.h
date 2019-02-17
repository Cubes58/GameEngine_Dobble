#pragma once

#include <memory>

#include "Window.h"
#include "Client.h"

class Scene;

enum class GameState : unsigned int {
	MENU = 0,
	ACTIVE,
	WIN,
	LOSE,
	SHUTDOWN
};

class Game {
private:
	Window &m_Window;
	Client m_Client;
	std::unique_ptr<Scene> m_Scene;
	GameState m_GameState;

	static constexpr const unsigned int m_s_NumberOfDifferentKeyCodes = 400;
	bool m_Keys[m_s_NumberOfDifferentKeyCodes];

public:
	Game(Window &p_Window);
	~Game();

	void ProcessEvents();
	void Update(float p_DeltaTime);
	void Render();
};