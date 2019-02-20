#include "Game.h"

#include "GLCore.hpp"

#include "Logger.h"

#include "EntityManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "PacketTypes.h"

#include "MainMenuScene.h"
#include "GamePlayScene.h"

Game::Game(Window &p_Window) : m_Window(p_Window), m_GameState(GameState::MAIN_MENU) {
	for (int i = 0; i < s_m_NumberOfDifferentKeyCodes; ++i) {
		m_Keys[i] = false;
	}
	SetScene();

	std::shared_ptr<RenderSystem> renderSystem = std::make_shared<RenderSystem>((float)m_Window.GetWidth(), (float)m_Window.GetHeight());
	EntityManagerInstance.AddSystem(renderSystem);
}

Game::~Game() {

}

void Game::ProcessEvents() {
	sf::Event event;
	while (m_Window.GetWindow().pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_Window.GetWindow().close();
			return;
		}
		else if (event.type == sf::Event::Resized)
			gl::Viewport(0, 0, event.size.width, event.size.height);
		else {
			unsigned int keyCode = event.key.code;
			if (keyCode >= 0 && keyCode < s_m_NumberOfDifferentKeyCodes) {
				if (event.type == sf::Event::KeyPressed) {
					m_Keys[keyCode] = gl::TRUE_;
					Log(MessageType::INFO) << "SFML key code: " << keyCode << " KEY PRESSED";
				}
				else if (event.type == sf::Event::KeyReleased) {
					m_Keys[keyCode] = gl::FALSE_;
					Log(MessageType::INFO) << "SFML key code: " << keyCode << " KEY RELEASED";
				}
			}
		}
	}
}

void Game::Update(float p_DeltaTime) {
	// Check whether something in the scene has caused it to change.
	if (m_Scene->Change(m_GameState))
		SetScene();	// Change the scene.

	m_Scene->Update(p_DeltaTime);
}

void Game::Render() {
	if (!m_Window.IsOpen())
		return;

	// Clear the window with grey.
	gl::ClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Draw game related stuff!
	m_Scene->Render(m_Window);

	// Switch the buffers.
	m_Window.GetWindow().display();
}

void Game::SetScene() {
	switch (m_GameState) {
	case GameState::MAIN_MENU:
		m_Scene = std::make_unique<MainMenuScene>();
		break;
	case GameState::ACTIVE:
		m_Scene = std::make_unique<GamePlayScene>();
		break;
	case GameState::WIN:

		break;
	case GameState::LOSE:

		break;
	case GameState::SHUTDOWN:

		break;
	default:
		m_GameState = GameState::MAIN_MENU;
		break;
	}
}