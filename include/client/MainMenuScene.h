#pragma once

#include "Scene.h"

class MainMenuScene : public Scene {
private:


public:
	MainMenuScene();
	~MainMenuScene() = default;

	virtual void HandleInputEvent(sf::Event &p_Event) override;
	virtual void Update(float p_DeltaTime) override;
	virtual void Render(Window &p_Window) override;
};