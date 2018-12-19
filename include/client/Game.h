#pragma once

#include <memory>

#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"

#include "Logger.h"

class Game {
private:
	Window &m_Window;
	//std::unique_ptr<Scene> m_Scene;

public:
	Game(Window &p_Window);
	~Game();

	void ProcessEvents();
	void Update(float p_DeltaTime);
	void Render();
};