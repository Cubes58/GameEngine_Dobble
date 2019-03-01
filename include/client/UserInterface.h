#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Vector2D.h"
#include "FontRenderer.h"

class Scene;
class Shape;
class Shader;

class UserInterface {
private:
	friend Scene;

	std::vector<std::shared_ptr<Shape>> m_Shapes;
	std::shared_ptr<FontRenderer> m_FontRenderer;
	std::shared_ptr<Shape> m_Background;	// Rectangle shape, for the background.
	Shader *m_Shader;

	void LoadUserInterfaceLayout(const std::string &p_File);

public:
	UserInterface(const Vector2D<float> &p_ScreenSize, const std::string &p_File);
	~UserInterface() = default;

	void Update(float p_DeltaTime);
	void Render();
};