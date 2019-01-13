#pragma once

#include <memory>

#include "Window.h"

class Scene;

enum class GameState : unsigned int {
	GAME_ACTIVE = 0,
	GAME_MENU,
	GAME_WIN,
	GAME_LOSE,
	GAME_SHUTDOWN
};

class Game {
private:
	Window &m_Window;
	GameState m_GameState;
	std::unique_ptr<Scene> m_Scene;

	static constexpr const unsigned int m_s_NumberOfDifferentKeyCodes = 400;
	bool m_Keys[m_s_NumberOfDifferentKeyCodes];

public:
	Game(Window &p_Window);
	~Game();

	void ProcessEvents();
	void Update(float p_DeltaTime);
	void Render();
};