#pragma once

#include <memory>
#include <string>

#include <glm/detail/type_vec3.hpp>
#include <SFML/Window/Event.hpp>

#include "Vector2D.h"
#include "GameState.h"
#include "Collision.h"

class Game;
class UserInterface;
class PostProcessor;
struct Text;

class Scene {
private:
	friend Game;

protected:
	Collision m_Collision;
	Vector2Df m_ScreenSize;
	GameState m_GameState;
	std::shared_ptr<UserInterface> m_UserInterface;
	std::shared_ptr<PostProcessor> m_PostProcessor;

public:
	Scene(const Vector2Df &p_ScreenSize, const std::string &p_File);
	virtual ~Scene() = default;

	virtual bool Change(GameState &p_GameState);
	virtual void HandleInputEvent(sf::Event &p_Event);
	virtual void Update(float p_DeltaTime);
	virtual void Render();

	void RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, const glm::vec3 &p_Colour);
	void RenderText(const std::string &p_Text, const Vector2Df &p_Position, float p_Scale, const glm::vec3 &p_Colour);

	void AddText(std::shared_ptr<Text> p_Text);
	void SetScreenSize(const Vector2Df &p_ScreenSize);
};