#include "MainMenuScene.h"

MainMenuScene::MainMenuScene() {

}

bool MainMenuScene::Change(GameState &p_GameState) {
	// If player clicks on the start game button, then change the game state to active!
	p_GameState = GameState::ACTIVE;
	return true;
}

void MainMenuScene::Update(float p_DeltaTime) {

}

void MainMenuScene::Render(Window &p_Window) {

}