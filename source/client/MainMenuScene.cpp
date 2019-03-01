#include "MainMenuScene.h"

#include <SFML/Window/Event.hpp>

MainMenuScene::MainMenuScene(const Vector2D<float> &p_ScreenSize, const std::string &p_File) 
	: Scene(p_ScreenSize, p_File) {
}

void MainMenuScene::HandleInputEvent(sf::Event &p_Event) {

}

void MainMenuScene::Update(float p_DeltaTime) {

}

void MainMenuScene::Render(Window &p_Window) {
	m_UserInterface->Render(p_Window);

	//RenderText("Main Menu", Vector2D<float>(0.5f, 0.5f), 0.55f, glm::vec3(0.2f, 0.5f, 0.1f));
}