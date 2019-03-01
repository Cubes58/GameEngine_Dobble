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

UserInterface::UserInterface(const Vector2D<float> &p_ScreenSize, const std::string &p_File) {
	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/arial.ttf", p_ScreenSize);
	m_Shader = ResourceManagerInstance.LoadShader("resources/shaders/ShapesShader.vert", "resources/shaders/ShapesShader.vert");

	m_Shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, p_ScreenSize.X(), p_ScreenSize.Y(), 0.0f, -1.0f, 1.0f);
	m_Shader->SetMat4("projection", projection);

	LoadUserInterfaceLayout(p_File);
}

void UserInterface::LoadUserInterfaceLayout(const std::string &p_File) {
	// Load UI information from a JSON file.
	// TEST...
	m_Shapes.reserve(1);
	m_Shapes.emplace_back(std::make_shared<CircleShape>(Vector2D<float>(240.0f, 360.0f), 64, 360, ResourceManagerInstance.GetTexture("apple")));

	m_Shapes.emplace_back(std::make_shared<RectangleShape>(Vector2D<float>(640.0f, 360.0f), Vector2D<float>(360.0f, 360.0f), ResourceManagerInstance.GetTexture("apple")));
}

void UserInterface::Update(float p_DeltaTime) {
	// Maybe add a timer.

}

void UserInterface::Render() {
	m_Shader->Use();
	for (auto &shape : m_Shapes) {
		shape->Render(*m_Shader);
	}
}