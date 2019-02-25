#include "MainMenuScene.h"

#include <SFML/Window/Event.hpp>

MainMenuScene::MainMenuScene() {

}

void MainMenuScene::HandleInputEvent(sf::Event &p_Event) {
	m_GameState = GameState::ACTIVE;
}

void MainMenuScene::Update(float p_DeltaTime) {

}

void MainMenuScene::Render(Window &p_Window) {

}