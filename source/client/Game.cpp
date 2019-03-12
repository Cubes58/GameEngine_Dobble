#include "Game.h"

#include "GLCore.hpp"
#include "Logger.h"

#include "EntityManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "PacketTypes.h"

#include "GamePlayScene.h"
#include "Text.h"

Game::Game(Window &p_Window) : m_Window(p_Window), m_GameState(GameState::MAIN_MENU) {
	EntityManagerInstance.Init();
	EntityManagerInstance.AddSystem(std::make_shared<RenderSystem>((float)m_Window.GetWidth(), (float)m_Window.GetHeight()));

	SetScene();
}

Game::~Game() { }

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
			//m_Window.SetScreenSize(event.size.width, event.size.height);
			break;
		case sf::Event::EventType::MouseMoved:
		case sf::Event::EventType::MouseButtonPressed:
		case sf::Event::EventType::MouseButtonReleased:
			mouseWorld = m_Window.GetWindow().mapPixelToCoords(sf::Mouse::getPosition(m_Window.GetWindow()));
			event.mouseMove.x = static_cast<int>(mouseWorld.x);
			event.mouseMove.y = static_cast<int>(mouseWorld.y);

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

	// Clear the window to black.
	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	m_Scene->Render();

	// Switch the buffers.
	m_Window.GetWindow().display();
}

void Game::SetScene() {
	Vector2Df screenSize = Vector2Df(static_cast<float>(m_Window.GetWidth()), static_cast<float>(m_Window.GetHeight()));

	float playerScore = 0;
	int numberOfRoundsWon = 0;
	if (m_PreviousState == GameState::ACTIVE) {
		// Once out of scope the raw pointer will be deleted.
		GamePlayScene *scene = static_cast<GamePlayScene*>(m_Scene.get());
		playerScore = scene->GetPlayerScore();
		numberOfRoundsWon = scene->GetRoundsWon();
	}

	switch (m_GameState) {
	case GameState::MAIN_MENU:
		m_Scene = std::make_unique<Scene>(screenSize, "resources/userInterfaceLayouts/MainMenuScene.JSON");
		break;
	case GameState::ACTIVE:
		m_Scene = std::make_unique<GamePlayScene>(screenSize, "resources/userInterfaceLayouts/GamePlayActiveLayout.JSON");
		break;
	case GameState::HELP:
		m_Scene = std::make_unique<Scene>(screenSize, "resources/userInterfaceLayouts/HelpLayout.JSON");
		break;
	case GameState::WIN:
		m_Scene = std::make_unique<Scene>(screenSize, "resources/userInterfaceLayouts/EndGameWinScene.JSON");
		break;
	case GameState::LOSE:
		m_Scene = std::make_unique<Scene>(screenSize, "resources/userInterfaceLayouts/EndGameLoseScene.JSON");
		break;
	case GameState::SHUTDOWN:
		break;
	default:
		m_GameState = GameState::MAIN_MENU;
		break;
	}

	m_PreviousState = this->m_GameState;
	m_Scene->m_GameState = this->m_GameState;

	auto SetEndGameText = [&](Scene *p_Scene, const glm::vec3 &p_Colour) {
		p_Scene->AddText(std::make_shared<Text>("Final Score: " + std::to_string((int)playerScore), Vector2Df(0.20f, 0.125f), 0.7f, p_Colour));
		p_Scene->AddText(std::make_shared<Text>("Rounds Won: " + std::to_string(numberOfRoundsWon), Vector2Df(0.60f, 0.125f), 0.7f, p_Colour));
	};

	if (m_GameState == GameState::WIN) {
		Scene *scene = static_cast<Scene*>(m_Scene.get());		
		SetEndGameText(scene, glm::vec3(0.098f, 0.439f, 0.098f));
	}
	else if (m_GameState == GameState::LOSE) {
		Scene *scene = static_cast<Scene*>(m_Scene.get());
		SetEndGameText(scene, glm::vec3(0.439f, 0.098f, 0.098f));
	}
}