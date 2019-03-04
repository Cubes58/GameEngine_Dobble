#pragma once

#include <string>
#include <vector>
#include <memory>

#include "Vector2D.h"
#include "FontRenderer.h"

#include "Shape.h"
#include "CircleShape.h"
#include "RectangleShape.h"
#include "Button.h"
#include "Text.h"

class Scene;
class Shader;

class UserInterface {
private:
	friend Scene;

	std::vector<std::shared_ptr<Button>> m_Buttons;
	std::vector<std::shared_ptr<Shape>> m_Shapes;
	std::vector<Text> m_Text;
	Vector2D<float> m_ScreenSize;
	float m_Time = 0.0f;
	std::shared_ptr<FontRenderer> m_FontRenderer;
	Shader *m_Shader;

	void LoadUserInterfaceLayout(const std::string &p_File);

public:
	UserInterface(const Vector2D<float> &p_ScreenSize, const std::string &p_File);
	~UserInterface() = default;

	void Update(float p_DeltaTime);
	void Render();

	const Vector2D<float> &GetScreenSize() const {
		return m_ScreenSize;
	}
	void SetScreenSize(const Vector2D<float> &p_ScreenSize) {
		m_ScreenSize = p_ScreenSize;
	}

	const float &Time() const {
		return m_Time;
	}
	void ResetTime() {
		m_Time = 0.0f;
	}

	std::vector<std::shared_ptr<Button>> &GetButtons() {
		return m_Buttons;
	}
};