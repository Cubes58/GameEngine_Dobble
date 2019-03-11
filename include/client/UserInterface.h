#pragma once

#include <string>
#include <vector>
#include <memory>

#include <SFML/Window/Event.hpp>

#include "Vector2D.h"
#include "GameState.h"

class Scene;
class Shader;
class FontRenderer;
class Shape;
struct Button;
struct Text;

class UserInterface {
private:
	friend Scene;

	std::vector<std::shared_ptr<Button>> m_Buttons;
	std::vector<std::shared_ptr<Shape>> m_Shapes;
	std::vector<std::shared_ptr<Text>> m_Text;
	Vector2Df m_ScreenSize;
	float m_Time = 0.0f;
	std::shared_ptr<FontRenderer> m_FontRenderer;
	Shader *m_Shader;

	void LoadUserInterfaceLayout(const std::string &p_File);

public:
	UserInterface(const Vector2Df &p_ScreenSize, const std::string &p_File);
	~UserInterface() = default;

	GameState HandleInput(sf::Event &p_Event, const GameState &p_CurrentGameState);
	void Update(float p_DeltaTime);
	void Render();

	const Vector2Df &GetScreenSize() const {
		return m_ScreenSize;
	}
	void SetScreenSize(const Vector2Df &p_ScreenSize) {
		m_ScreenSize = p_ScreenSize;
	}

	const float &Time() const {
		return m_Time;
	}
	void ResetTime() {
		m_Time = 0.0f;
	}

	std::vector<std::shared_ptr<Button>> &GetButtons() {
		return m_Buttons;
	}
};