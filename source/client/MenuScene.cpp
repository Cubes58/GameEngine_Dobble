#include "MenuScene.h"

#include <SFML/Window/Event.hpp>

#include "Logger.h"

MenuScene::MenuScene(const Vector2D<float> &p_ScreenSize, const std::string &p_File)
	: Scene(p_ScreenSize, p_File) {
}

void MenuScene::HandleInputEvent(sf::Event &p_Event) {
	Vector2D<float> mousePosition((float)p_Event.mouseMove.x, (float)p_Event.mouseMove.y);
	for (auto &button : m_UserInterface->GetButtons()) {
		if (button->m_ShapeType == typeid(RectangleShape)) {
			std::shared_ptr<RectangleShape> shape = std::static_pointer_cast<RectangleShape>(button->m_Shape);
			if (m_Collision(shape->GetPosition(), shape->GetSize(), mousePosition)) {
				if (p_Event.type == p_Event.MouseButtonPressed)
					m_GameState = button->m_GameState;
			}
			else
				;//Animate button/Set colour mask.
		}
		else {
			std::shared_ptr<CircleShape> shape = std::static_pointer_cast<CircleShape>(button->m_Shape);
			if (m_Collision(shape->GetPosition(), shape->GetRadius(), mousePosition)) {
				if (p_Event.type == p_Event.MouseButtonPressed)
					m_GameState = button->m_GameState;
			}
			else
				;//Animate button/Set colour mask.
		}
	}

}

void MenuScene::Update(float p_DeltaTime) {
	m_UserInterface->Update(p_DeltaTime);
}

void MenuScene::Render(Window &p_Window) {
	m_UserInterface->Render();
}