#include "Scene.h"

#include "UserInterface.h"
#include "FontRenderer.h"
#include "PostProcessor.h"

Scene::Scene(const Vector2Df &p_ScreenSize, const std::string &p_File) : m_ScreenSize(p_ScreenSize) {
	m_UserInterface = std::make_shared<UserInterface>(m_ScreenSize, p_File);
	m_PostProcessor = std::make_shared<PostProcessor>(m_ScreenSize);
}

bool Scene::Change(GameState &p_GameState) {
	if (static_cast<unsigned int>(m_GameState) != static_cast<unsigned int>(p_GameState)) {
		p_GameState = m_GameState;
		return true;
	}

	return false;
}

void Scene::HandleInputEvent(sf::Event &p_Event) {
	m_GameState = m_UserInterface->HandleInput(p_Event, m_GameState);
}

void Scene::Update(float p_DeltaTime) {
	m_UserInterface->Update(p_DeltaTime);
}

void Scene::Render() {
	m_UserInterface->Render();
}

void Scene::RenderText(const std::string &p_Text, float p_XPosition, float p_YPosition, float p_Scale, const glm::vec3 &p_Colour) {
	m_UserInterface->m_FontRenderer->RenderText(p_Text, p_XPosition, p_YPosition, p_Scale, p_Colour);
}
void Scene::RenderText(const std::string &p_Text, const Vector2Df &p_Position, float p_Scale, const glm::vec3 &p_Colour) {
	m_UserInterface->m_FontRenderer->RenderText(p_Text, p_Position.X(), p_Position.Y(), p_Scale, p_Colour);
}

void Scene::AddText(std::shared_ptr<Text> p_Text) {
	m_UserInterface->m_Text.emplace_back(p_Text);
}

void Scene::SetScreenSize(const Vector2Df &p_ScreenSize) {
	m_ScreenSize = p_ScreenSize;

	m_PostProcessor = std::make_shared<PostProcessor>(m_ScreenSize);
}

void Scene::SetGameState(const GameState &p_GameState) {
	m_GameState = p_GameState;
}