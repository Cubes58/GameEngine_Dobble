#include "EndGameScene.h"

EndGameScene::EndGameScene(const Vector2D<float> &p_ScreenSize, const std::string &p_File) 
	: MenuScene(p_ScreenSize, p_File) {
}

void EndGameScene::Render(Window &p_Window) {
	m_UserInterface->Render();

	RenderText("Final Score: " + std::to_string(static_cast<int>(m_FinalPlayerScore)), Vector2D<float>(0.20f, 0.125f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f));
	RenderText("Rounds Won: " + std::to_string(static_cast<int>(m_NumberOfRoundsWon)), Vector2D<float>(0.60f, 0.125f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f));
}