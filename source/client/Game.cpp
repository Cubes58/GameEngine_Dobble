#include "Game.h"

#include "GLCore.hpp"
#include "Logger.h"

#include "EntityManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "PacketTypes.h"

#include "MenuScene.h"
#include "GamePlayScene.h"
#include "EndGameScene.h"

Game::Game(Window &p_Window) : m_Window(p_Window), m_GameState(GameState::MAIN_MENU) {
	EntityManagerInstance.Init();
	EntityManagerInstance.AddSystem(std::make_shared<RenderSystem>((float)m_Window.GetWidth(), (float)m_Window.GetHeight()));

	SetScene();
}

Game::~Game() {

}

void Game::ProcessEvents() {
	sf::Event event;
	sf::Vector2f mouseWorld;
	while (m_Window.GetWindow().pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			m_Window.GetWindow().close();
			return;
			break;
		case sf::Event::Resized:
			gl::Viewport(0, 0, event.size.width, event.size.height);
			m_Scene->SetScreenSize(Vector2Df(static_cast<float>(event.size.width), static_cast<float>(event.size.height)));
			break;
		case sf::Event::EventType::MouseButtonPressed:
		case sf::Event::EventType::MouseButtonReleased:
		case sf::Event::EventType::MouseMoved:		
			mouseWorld = m_Window.GetWindow().mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
			event.mouseButton.x = static_cast<int>(mouseWorld.x);
			event.mouseButton.y = static_cast<int>(mouseWorld.y);

			m_Scene->HandleInputEvent(event);
			break;

		default:
			break;
		}
	}
}

void Game::Update(float p_DeltaTime) {
	// Check whether something in the scene has caused it to change.
	if (m_Scene->Change(m_GameState))
		SetScene();	// Change the scene.

	m_Scene->Update(p_DeltaTime);

	if (m_GameState == GameState::SHUTDOWN)
		m_Window.Close();
}

void Game::Render() {
	if (!m_Window.IsOpen())
		return;

	// Clear the window with grey.
	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Draw game related stuff!
	m_Scene->Render(m_Window);

	// Switch the buffers.
	m_Window.GetWindow().display();
}

void Game::SetScene() {
	switch (m_GameState) {
	case GameState::MAIN_MENU:
		m_Scene = std::make_unique<MenuScene>(Vector2D<float>((float)m_Window.GetWidth(), (float)m_Window.GetHeight()), "resources/userInterfaceLayouts/MainMenuScene.JSON");
		break;
	case GameState::ACTIVE:
		m_Scene = std::make_unique<GamePlayScene>(Vector2D<float>((float)m_Window.GetWidth(), (float)m_Window.GetHeight()), "resources/userInterfaceLayouts/GamePlayActiveLayout.JSON");
		break;
	case GameState::HELP:
		m_Scene = std::make_unique<MenuScene>(Vector2D<float>((float)m_Window.GetWidth(), (float)m_Window.GetHeight()), "resources/userInterfaceLayouts/HelpLayout.JSON");
		break;
	case GameState::WIN:
		m_Scene = std::make_unique<EndGameScene>(Vector2D<float>((float)m_Window.GetWidth(), (float)m_Window.GetHeight()), "resources/userInterfaceLayouts/EndGameWinScene.JSON");
		break;
	case GameState::LOSE:
		m_Scene = std::make_unique<EndGameScene>(Vector2D<float>((float)m_Window.GetWidth(), (float)m_Window.GetHeight()), "resources/userInterfaceLayouts/EndGameLoseScene.JSON");
		break;
	case GameState::SHUTDOWN:
		break;
	default:
		m_GameState = GameState::MAIN_MENU;
		break;
	}
	m_Scene->m_GameState = this->m_GameState;
}