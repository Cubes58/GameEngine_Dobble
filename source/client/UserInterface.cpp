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

UserInterface::UserInterface(const Vector2Df &p_ScreenSize, const std::string &p_File) : m_ScreenSize(p_ScreenSize) {
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
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2Df(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2Df(m_ScreenSize.X(), m_ScreenSize.Y()), ResourceManagerInstance.GetTexture("menuBackgroundCottage")));
		
		m_Buttons.reserve(3);
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2Df(520.0f, 180.0f), Vector2Df(280.0f, 90.0f), 
			ResourceManagerInstance.GetTexture("paintStroke5")), typeid(RectangleShape), GameState::ACTIVE));
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2Df(520.0f, 300.0f), Vector2Df(280.0f, 90.0f), 
			ResourceManagerInstance.GetTexture("paintStroke10")), typeid(RectangleShape), GameState::HELP));
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2Df(520.0f, 420.0f), Vector2Df(280.0f, 90.0f),
			ResourceManagerInstance.GetTexture("paintStroke3")), typeid(RectangleShape), GameState::SHUTDOWN));

		m_Text.reserve(3);
		m_Text.emplace_back("Play", Vector2Df(0.36f, 0.735f), 1.0f, glm::vec3(0.098f, 0.098f, 0.439f));
		m_Text.emplace_back("Help", Vector2Df(0.36f, 0.565f), 1.0f, glm::vec3(0.098f, 0.098f, 0.439f));
		m_Text.emplace_back("Quit", Vector2Df(0.36f, 0.385f), 1.0f, glm::vec3(0.098f, 0.098f, 0.439f));
	}
	else if (p_File == "resources/userInterfaceLayouts/GamePlayActiveLayout.JSON") {
		m_Shapes.reserve(1);
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2Df(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2Df(m_ScreenSize.X(), m_ScreenSize.Y()), ResourceManagerInstance.GetTexture("activeBackground")));

		m_Buttons.reserve(1);
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2Df(90.0f, 680.0f), Vector2Df(140.0f, 45.0f),
			ResourceManagerInstance.GetTexture("paintStroke")), typeid(RectangleShape), GameState::MAIN_MENU));

		m_Text.reserve(1);
		m_Text.emplace_back("Leave", Vector2Df(0.03f, 0.04f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f));
	}
	else if (p_File == "resources/userInterfaceLayouts/EndGameWinScene.JSON") {
		m_Shapes.reserve(1);
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2Df(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2Df(m_ScreenSize.X(), m_ScreenSize.Y()), ResourceManagerInstance.GetTexture("oceanMountainBackground")));

		m_Buttons.reserve(1);
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2Df(90.0f, 680.0f), Vector2Df(140.0f, 45.0f),
			ResourceManagerInstance.GetTexture("paintStroke2")), typeid(RectangleShape), GameState::MAIN_MENU));

		m_Text.reserve(3);
		m_Text.emplace_back("Menu", Vector2Df(0.03f, 0.04f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f));
		m_Text.emplace_back("You have won.", Vector2Df(0.40f, 0.5f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f));
		m_Text.emplace_back("Congratulations!", Vector2Df(0.38f, 0.44f), 0.7f, glm::vec3(0.098f, 0.439f, 0.098f));
	}
	else if (p_File == "resources/userInterfaceLayouts/EndGameLoseScene.JSON") {
		m_Shapes.reserve(1);
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2Df(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2Df(m_ScreenSize.X(), m_ScreenSize.Y()), ResourceManagerInstance.GetTexture("oceanMountainBackground")));

		m_Buttons.reserve(1);
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2Df(90.0f, 680.0f), Vector2Df(140.0f, 45.0f),
			ResourceManagerInstance.GetTexture("paintStroke2")), typeid(RectangleShape), GameState::MAIN_MENU));

		m_Text.reserve(3);
		m_Text.emplace_back("Menu", Vector2Df(0.03f, 0.04f), 0.7f, glm::vec3(0.439f, 0.098f, 0.098f));
		m_Text.emplace_back("You have lost.", Vector2Df(0.385f, 0.5f), 0.7f, glm::vec3(0.439f, 0.098f, 0.098f));
		m_Text.emplace_back("Better luck next time!", Vector2Df(0.345f, 0.44f), 0.7f, glm::vec3(0.439f, 0.098f, 0.098f));
	}
	else /* Help menu. */ {
		m_Shapes.reserve(2);
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2Df(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2Df(m_ScreenSize.X(), m_ScreenSize.Y()), ResourceManagerInstance.GetTexture("oceanFloor")));
		m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2Df(m_ScreenSize.X() / 2.0f, m_ScreenSize.Y() / 2.0f),
			Vector2Df(m_ScreenSize.X() / 1.2f, m_ScreenSize.Y() / 1.35f), ResourceManagerInstance.GetTexture("paintedBlock")));

		m_Buttons.reserve(1);
		m_Buttons.emplace_back(std::make_shared<Button>(std::make_shared<RectangleShape>(Vector2Df(90.0f, 680.0f), Vector2Df(140.0f, 65.0f),
			ResourceManagerInstance.GetTexture("paintStroke")), typeid(RectangleShape), GameState::MAIN_MENU));

		m_Text.reserve(4);
		m_Text.emplace_back("Back", Vector2Df(0.03f, 0.04f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
		m_Text.emplace_back("Help:", Vector2Df(0.135f, 0.78f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
		m_Text.emplace_back("Vote for 6 or 8 symbols per card.", Vector2Df(0.135f, 0.72f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
		m_Text.emplace_back("Two cards are displayed, one is unique to you, and", Vector2Df(0.135f, 0.66f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
		m_Text.emplace_back("the other is the deck card. There will always be one,", Vector2Df(0.135f, 0.6f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
		m_Text.emplace_back("and only one symbol that matches on both cards.", Vector2Df(0.135f, 0.54f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
		m_Text.emplace_back("Click on that symbol, before any other player, and", Vector2Df(0.135f, 0.48f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
		m_Text.emplace_back("win the round, againing score.", Vector2Df(0.135f, 0.42f), 0.7f, glm::vec3(0.2902f, 0.7765f, 0.8f));
	}
}

void UserInterface::Update(float p_DeltaTime) {
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