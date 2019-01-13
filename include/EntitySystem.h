#pragma once

#include <memory>

#include "Window.h"

class EntitySystem {
public:
	EntitySystem() = default;
	virtual ~EntitySystem() = default;

	virtual void Update(float p_DeltaTime) {}
	virtual void Render(Window &p_Window) {}
};
