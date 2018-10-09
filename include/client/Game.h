#pragma once

#include "Window.h"

#include "Logger.h"

class Game {
private:
	Window &m_Window;

public:
	Game(Window &p_Window);
	~Game();

	void ProcessEvents();
	void Update(float p_DeltaTime);
	void Render();
};