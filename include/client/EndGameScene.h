#pragma once

#include "MenuScene.h"

class EndGameScene : public MenuScene {
private:
	float m_FinalPlayerScore;
	unsigned int m_NumberOfRoundsWon;

public:
	EndGameScene(const Vector2D<float> &p_ScreenSize, const std::string &p_File);
	~EndGameScene() = default;

	virtual void Render(Window &p_Window) override;

	void SetFinalPlayerScore(float p_FinalPlayerScore) {
		m_FinalPlayerScore = p_FinalPlayerScore;
	}

	void SetNumberOfRoundsWon(unsigned int p_NumberOfRoundsWon) {
		m_NumberOfRoundsWon = p_NumberOfRoundsWon;
	}
};