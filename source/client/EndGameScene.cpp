#include "EndGameScene.h"

#include "UserInterface.h"
#include "FontRenderer.h"

#include "Text.h"

EndGameScene::EndGameScene(const Vector2Df &p_ScreenSize, const std::string &p_File) 
	: Scene(p_ScreenSize, p_File) {
}

void EndGameScene::Render() {
	m_UserInterface->Render();

	AddText(std::make_shared<ReferenceText<float>>("Final Score: ", m_FinalPlayerScore, Vector2Df(0.20f, 0.125f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f)));
	AddText(std::make_shared<ReferenceText<unsigned int>>("Rounds Won: ", m_NumberOfRoundsWon, Vector2Df(0.60f, 0.125f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f)));
}