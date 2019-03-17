#include "UserInterface.h"

#include "GLCore.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <fstream>
#include <typeindex>
#include <json/json.h>

#include "ResourceManager.h"
#include "FontRenderer.h"
#include "Shader.h"
#include "Collision.h"

#include "CircleShape.h"
#include "RectangleShape.h"
#include "Button.h"
#include "Text.h"

#include "Logger.h"

UserInterface::UserInterface(const Vector2Df &p_ScreenSize, const std::string &p_File) : m_ScreenSize(p_ScreenSize) {
	m_FontRenderer = std::make_shared<FontRenderer>("resources/fonts/FingerPaint-Regular.ttf", p_ScreenSize);
	m_Shader = ResourceManagerInstance.LoadShader("resources/shaders/ShapesShader.vert", "resources/shaders/ShapesShader.frag");

	m_Shader->Use();
	glm::mat4 projection = glm::ortho(0.0f, p_ScreenSize.X(), p_ScreenSize.Y(), 0.0f, -1.0f, 1.0f);
	m_Shader->SetMat4("projection", projection);

	LoadUserInterfaceLayout(p_File);
}

bool UserInterface::LoadUserInterfaceLayout(const std::string &p_File) {
	// Load UI information from a JSON file.
	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	try {
		jsonData.open(p_File.c_str());
		// Check for errors, when parsing the file data.
		if (!reader.parse(jsonData, root)) {
			Log(Type::FAULT) << "Failed to parse data from: " << p_File << reader.getFormattedErrorMessages() << "\n";
			throw std::invalid_argument("Couldn't parse file.");
		}

		if (jsonData.is_open())
			jsonData.close();
	}
	catch (const std::invalid_argument &p_Exception) {
		Log(Type::FAULT) << "INVALID_ARGUMENT: " << p_Exception.what() << "\n";
		if (jsonData.is_open())
			jsonData.close();
		return false;
	}
	catch (...) {
		// Catch all rune-time errors, and attempt to recover.
		Log(Type::FAULT) << "Something unexpected happened, while reading the user interface file: " << p_File;
		if (jsonData.is_open())
			jsonData.close();
		return false;
	}
	const Json::Value userInterfaceObjects = root["UserInterfaceObjects"];

	for (unsigned int i = 0; i < userInterfaceObjects.size(); i++) {
		// Get the object's type name.
		const Json::Value typeNode = userInterfaceObjects[i]["type"];
		std::string typeName = "default";
		if (typeNode.type() != Json::nullValue) {
			typeName = typeNode.asString();
		}

		// Get the object's shape name.
		const Json::Value shapeNode = userInterfaceObjects[i]["shapeType"];
		std::string shapeName = "default";
		if (shapeNode.type() != Json::nullValue) {
			shapeName = shapeNode.asString();
		}

		// Get the position node.
		Vector2Df position(0.0f);
		const Json::Value positionNode = userInterfaceObjects[i]["position"];
		if (positionNode.type() != Json::nullValue) {
			position.SetX(positionNode[0].asFloat());
			position.SetY(positionNode[1].asFloat());
		}

		// Get the orientation node.
		Vector2Df orientation(0.0f);
		const Json::Value orientationNode = userInterfaceObjects[i]["orientation"];
		if (orientationNode.type() != Json::nullValue) {
			orientation.SetX(orientationNode[0].asFloat());
			orientation.SetY(orientationNode[1].asFloat());
		}

		// Get the size node.
		Vector2Df size(0.0f);
		const Json::Value sizeNode = userInterfaceObjects[i]["size"];
		if (sizeNode.type() != Json::nullValue) {
			size.SetX(sizeNode[0].asFloat());
			size.SetY(sizeNode[1].asFloat());
		}

		// Get the object texture's name.
		const Json::Value textureNode = userInterfaceObjects[i]["texture"];
		std::string textureName = "default";
		if (textureNode.type() != Json::nullValue) {
			textureName = textureNode.asString();
		}

		// Get the state the game changes to, if the object is interacted with.
		const Json::Value gameStateNode = userInterfaceObjects[i]["stateChange"];
		std::string gameStateName = "default";
		if (gameStateNode.type() != Json::nullValue) {
			gameStateName = gameStateNode.asString();
		}

		// Get the object's text.
		const Json::Value textNode = userInterfaceObjects[i]["text"];
		std::string textData = "default";
		if (textNode.type() != Json::nullValue) {
			textData = textNode.asString();
		}

		// Get the object's scale.
		const Json::Value scaleNode = userInterfaceObjects[i]["scale"];
		float scale = 1.0f;
		if (scaleNode.type() != Json::nullValue) {
			scale = scaleNode.asFloat();
		}

		// Get the object's radius.
		const Json::Value radiusNode = userInterfaceObjects[i]["radius"];
		float radius = -1.0f;
		if (radiusNode.type() != Json::nullValue) {
			radius = radiusNode.asFloat();
		}

		// Get the object's radius.
		const Json::Value numberOfFacesNode = userInterfaceObjects[i]["numberOfFaces"];
		unsigned int numberOfFaces = 64;
		if (numberOfFacesNode.type() != Json::nullValue) {
			numberOfFaces = numberOfFacesNode.asUInt();
		}

		// Get the object's colour.
		glm::vec3 colour(1.0f, 1.0f, 1.0f);
		const Json::Value colourNode = userInterfaceObjects[i]["colour"];
		if (colourNode.type() != Json::nullValue) {
			colour.r = colourNode[0].asFloat();
			colour.g = colourNode[1].asFloat();
			colour.b = colourNode[2].asFloat();
		}
		
		Texture2D *texture = nullptr;
		if (textureName != "Null" || textureName != "NULL" || textureName != "null" || textureName != "Nullptr" || textureName != "nullptr")
			texture = ResourceManagerInstance.GetTexture(textureName);


		std::shared_ptr<Shape> shapeObject;
		std::type_index shapeType = typeid(RectangleShape);
		if (shapeName == "rectangleShape" || shapeName == "RectangleShape") {
			shapeType = typeid(RectangleShape);
			shapeObject = std::make_shared<RectangleShape>(position, size, texture);
		}
		else if (shapeName == "circleShape" || shapeName == "CircleShape") {
			shapeType = typeid(CircleShape);
			shapeObject = std::make_shared<CircleShape>(position, numberOfFaces, radius, texture);
		}

		if (typeName == "shape") {
			m_Shapes.emplace_back(shapeObject);
		}
		else if (typeName == "button") {
			m_Buttons.emplace_back(std::make_shared<Button>(shapeObject, shapeType, GetGameStateFromName(gameStateName)));
		}
		else if (typeName == "text") {
			m_Text.emplace_back(std::make_shared<Text>(textData, position, scale, colour));
		}
		else {
			Log(Type::FAULT) << "Unknown type, please check the JSON file: " << p_File;
		}
	}

	return true;
}

GameState UserInterface::HandleInput(sf::Event &p_Event, const GameState &p_CurrentGameState) {
	static Collision collision;

	Vector2Df mousePosition((float)p_Event.mouseMove.x, (float)p_Event.mouseMove.y);
	bool highlightButton = false;
	unsigned int buttonToHighlightIndex = 0;
	for (int i = 0; i < m_Buttons.size(); ++i) {
		if (m_Buttons[i]->m_ShapeType == typeid(RectangleShape)) {
			std::shared_ptr<RectangleShape> shape = std::static_pointer_cast<RectangleShape>(m_Buttons[i]->m_Shape);
			if (collision(shape->GetPosition(), shape->GetSize(), mousePosition)) {
				if (p_Event.type == p_Event.MouseButtonPressed)
					return m_Buttons[i]->m_GameState;
				else {
					highlightButton = true;
					buttonToHighlightIndex = i;
				}
			}
		}
		else {
			std::shared_ptr<CircleShape> shape = std::static_pointer_cast<CircleShape>(m_Buttons[i]->m_Shape);
			if (collision(shape->GetPosition(), shape->GetRadius(), mousePosition)) {
				if (p_Event.type == p_Event.MouseButtonPressed)
					return m_Buttons[i]->m_GameState;
				else {
					highlightButton = true;
					buttonToHighlightIndex = i;
				}
			}
		}
	}

	if (highlightButton) {
		m_Buttons[buttonToHighlightIndex]->m_Shape->SetColourTextureMixValue(0.75f);
		m_Buttons[buttonToHighlightIndex]->m_Shape->SetColour(glm::vec4(0.75f, 0.75f, 0.75f, 1.0f));
		m_Buttons[buttonToHighlightIndex]->m_Shape->SetUsingColourAsTextureMask(true);
	}
	else {
		// Due to not knowing which button had been highlighted they all need to be iterated through, and reset.
		for (int i = 0; i < m_Buttons.size(); ++i) {
			m_Buttons[i]->m_Shape->SetColourTextureMixValue(0.0f);
		}
	}

	return p_CurrentGameState;
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
		m_FontRenderer->RenderText(*text);
	}
}

GameState UserInterface::GetGameStateFromName(const std::string &p_GameStateName) {
	if (p_GameStateName == "MAIN_MENU") {
		return GameState::MAIN_MENU;
	}
	else if (p_GameStateName == "ACTIVE") {
		return GameState::ACTIVE;
	}
	else if (p_GameStateName == "HELP") {
		return GameState::HELP;
	}
	else if (p_GameStateName == "WIN") {
		return GameState::WIN;
	}
	else if (p_GameStateName == "LOSE") {
		return GameState::LOSE;
	}
	else if (p_GameStateName == "SHUTDOWN") {
		return GameState::SHUTDOWN;
	}
	Log(Type::WARNING) << "The button state couldn't be found, returning the NOT_AVILABLE state. Make sure this is what you want.";

	return GameState::NOT_AVAILABLE;
}