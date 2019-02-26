#pragma once

#include "Scene.h"

class EndGameScreen : public Scene {
private:


public:
	EndGameScreen(const Vector2D<float> &p_ScreenSize, const std::string &p_File);
	~EndGameScreen() = default;

	virtual void HandleInputEvent(sf::Event &p_Event) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render(Window &p_Window) override;
};