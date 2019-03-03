#include "UserInterface.h"

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <fstream>
#include <json/json.h>

#include "ResourceManager.h"
#include "Shader.h"

#include "Shape.h"
#include "CircleShape.h"
#include "RectangleShape.h"
#include "Button.h"

UserInterface::UserInterface(const Vector2D<float> &p_ScreenSize, const std::string &p_File) : m_ScreenSize(p_ScreenSize) {
	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/FingerPaint-Regular.ttf", p_ScreenSize);
	m_Shader = ResourceManagerInstance.LoadShader("resources/shaders/ShapesShader.vert", "resources/shaders/ShapesShader.frag");

	m_Shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, p_ScreenSize.X(), p_ScreenSize.Y(), 0.0f, -1.0f, 1.0f);
	m_Shader->SetMat4("projection", projection);

	LoadUserInterfaceLayout(p_File);
}

void UserInterface::LoadUserInterfaceLayout(const std::string &p_File) {
	// Load UI information from a JSON file.
	if (p_File == "resources/userInterfaceLayouts/MainMenuScene.JSON") {
		m_Shapes.reserve(1);
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2D<float>(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2D<float>(m_ScreenSize.X(), m_ScreenSize.Y()), ResourceManagerInstance.GetTexture("menuBackgroundCottage")));
		
		m_Buttons.reserve(3);
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2D<float>(520.0f, 180.0f), Vector2D<float>(280.0f, 90.0f), 
			ResourceManagerInstance.GetTexture("whiteRectangle")), typeid(RectangleShape), GameState::ACTIVE));
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2D<float>(520.0f, 300.0f), Vector2D<float>(280.0f, 90.0f), 
			ResourceManagerInstance.GetTexture("whiteRectangle")), typeid(RectangleShape), GameState::HELP));
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2D<float>(520.0f, 420.0f), Vector2D<float>(280.0f, 90.0f),
			ResourceManagerInstance.GetTexture("whiteRectangle")), typeid(RectangleShape), GameState::SHUTDOWN));
	}
	else if (p_File == "resources/userInterfaceLayouts/GamePlayActiveLayout.JSON") {
		m_Shapes.reserve(1);
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2D<float>(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2D<float>(m_ScreenSize.X(), m_ScreenSize.Y()), ResourceManagerInstance.GetTexture("activeBackground")));

		m_Buttons.reserve(1);
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2D<float>(320.0f, 780.0f), Vector2D<float>(280.0f, 90.0f),
			ResourceManagerInstance.GetTexture("whiteRectangle")), typeid(RectangleShape), GameState::MAIN_MENU));
	}
	else if (p_File == "resources/userInterfaceLayouts/EndGameWinScene.JSON") {

	}
	else if (p_File == "resources/userInterfaceLayouts/EndGameLoseScene.JSON") {

	}
	else /* Help menu. */ {

	}
}

void UserInterface::Update(float p_DeltaTime) {
	// Maybe add a timer.
	m_Time += p_DeltaTime;
}

void UserInterface::Render() {
	m_Shader->Use();
	for (auto &shape : m_Shapes) {
		shape->Render(*m_Shader);
	}

	for (auto &button : m_Buttons) {
		button->m_Shape->Render(*m_Shader);
	}

	for (auto &text : m_Text) {
		m_FontRenderer->RenderText(text);
	}
}