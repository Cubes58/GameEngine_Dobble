#pragma once

#include "Scene.h"

class EndGameScene : public Scene {
private:
	float m_FinalPlayerScore;
	unsigned int m_NumberOfRoundsWon;

public:
	EndGameScene(const Vector2Df &p_ScreenSize, const std::string &p_File);
	~EndGameScene() = default;

	virtual void Render() override;

	void SetFinalPlayerScore(float p_FinalPlayerScore) {
		m_FinalPlayerScore = p_FinalPlayerScore;
	}

	void SetNumberOfRoundsWon(unsigned int p_NumberOfRoundsWon) {
		m_NumberOfRoundsWon = p_NumberOfRoundsWon;
	}
};