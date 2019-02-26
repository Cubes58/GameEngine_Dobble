#pragma once

#include <memory>
#include <string>

#include <glm/detail/type_vec3.hpp>

#include "Window.h"
#include "GameState.h"
#include "Vector2D.h"

#include "UserInterface.h"
#include "Collision.h"

class Game;
class sf::Event;

class Scene {
private:
	friend Game;

protected:
	std::shared_ptr<UserInterface> m_UserInterface;
	Collision m_Collision;
	Vector2D<float> m_ScreenSize;
	GameState m_GameState;

public:
	Scene(const Vector2D<float> &p_ScreenSize, const std::string &p_File) : m_ScreenSize(p_ScreenSize) {
		m_UserInterface = std::make_shared<UserInterface>(m_ScreenSize, p_File);
	}
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

	void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, const glm::vec3 &p_Colour) {
		m_UserInterface->m_FontRenderer->RenderText(p_Text, p_XPosition, p_YPosition, p_Scale, p_Colour);
	}
	void RenderText(const std::string &p_Text, const Vector2D<float> &p_Position, float p_Scale, const glm::vec3 &p_Colour) {
		m_UserInterface->m_FontRenderer->RenderText(p_Text, p_Position.X(), p_Position.Y(), p_Scale, p_Colour);
	}
};