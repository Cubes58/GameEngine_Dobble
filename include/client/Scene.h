#pragma once

#include "Window.h"
#include "GameState.h"

class Scene {
public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual bool Change(GameState &p_GameState) = 0;

	virtual void Update(float p_DeltaTime) = 0;
	virtual void Render(Window &p_Window) = 0;
};