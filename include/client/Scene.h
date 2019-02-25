#pragma once

#include "Window.h"
#include "GameState.h"
#include "Collision.h"

class sf::Event;

class Scene {
protected:
	Collision m_Collision;
	GameState m_GameState;

public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual bool Change(GameState &p_GameState) {
		if (static_cast<unsigned int>(m_GameState) != static_cast<unsigned int>(p_GameState)) {
			p_GameState = m_GameState;
			return true;
		}

		return false;
	}

	virtual void HandleInputEvent(sf::Event &p_Event) = 0;
	virtual void Update(float p_DeltaTime) = 0;
	virtual void Render(Window &p_Window) = 0;
};