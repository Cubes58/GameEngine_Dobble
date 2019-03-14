#pragma once

#include <memory>

#include "Window.h"
#include "GameState.h"

class Scene;

class Game {
private:
	Window &m_Window;
	std::unique_ptr<Scene> m_Scene;
	GameState m_GameState;
	GameState m_PreviousState;

	void SetScene();

public:
	Game(Window &p_Window);
	~Game();

	void ProcessEvents();
	void Update(float p_DeltaTime);
	void Render();
};