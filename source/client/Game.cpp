#include "Game.h"

#include "GLCore.hpp"

#include "EntityManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "Logger.h"
#include "Scene.h"

Game::Game(Window &p_Window) : m_Window(p_Window), m_GameState(GameState::GAME_ACTIVE) {
	for (int i = 0; i < m_s_NumberOfDifferentKeyCodes; ++i) {
		m_Keys[i] = false;
	}


	EntityManagerInstance.Init();
	EntityManager::Instance().CreateEntity("EntityOne");
	PositionComponent positionComponent;
	positionComponent.m_XPosition = 5.5;
	positionComponent.m_YPosition = 1.75;
	positionComponent.m_Width = 500;
	positionComponent.m_Height = 500;
	positionComponent.m_Rotation = 0;
	EntityManager::Instance().AddComponentToEntity("EntityOne", positionComponent);

	RenderComponent renderComponent;
	ResourceManager::LoadTexture("resources/images/test.jpg", gl::FALSE_, "CardBackground");
	renderComponent.m_CardBackgroundTextureID = "CardBackground";
	EntityManager::Instance().AddComponentToEntity("EntityOne", renderComponent);

	//EntityManager::Instance().RemoveEntity("EntityOne");
	std::weak_ptr<PositionComponent> posComp = EntityManagerInstance.GetComponent<PositionComponent>("EntityOne");

	auto pointer = posComp.lock();
	std::cout << "\n\n" << pointer->m_XPosition << std::endl;
	std::cout << pointer->m_YPosition << std::endl;

	std::shared_ptr<RenderSystem> renderSystem = std::make_shared<RenderSystem>(m_Window);
	EntityManagerInstance.AddSystem(renderSystem);
}

Game::~Game() {

}

void Game::ProcessEvents() {
	sf::Event event;
	while (m_Window.GetWindow().pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_Window.GetWindow().close();	// Causes an OpenGL error.
			return;
		}
		else if (event.type == sf::Event::Resized)
			gl::Viewport(0, 0, event.size.width, event.size.height);
		else {
			unsigned int keyCode = event.key.code;
			if (keyCode >= 0 && keyCode < m_s_NumberOfDifferentKeyCodes) {
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
	// TODO: Invoke entity systems
	//EntityManagerInstance.InvokeSystems();

	GLenum e;
	while ((e = gl::GetError()) != gl::NO_ERROR_) {
		switch (e) {
		case gl::INVALID_ENUM:
			Log(MessageType::FAULT) << "ERROR: GL_INVALID_ENUM";
			ASSERT(false);
			break;
		case gl::INVALID_VALUE:
			Log(MessageType::FAULT) << "ERROR: GL_INVALID_VALUE";
			ASSERT(false);
			break;
		case gl::INVALID_OPERATION:
			Log(MessageType::FAULT) << "ERROR: GL_INVALID_OPERATION";
			ASSERT(false);
			break;
		case gl::INVALID_FRAMEBUFFER_OPERATION:
			Log(MessageType::FAULT) << "ERROR: GL_INVALID_FRAMEBUFFER_OPERATION";
			ASSERT(false);
			break;
		case gl::OUT_OF_MEMORY:
			Log(MessageType::FAULT) << "ERROR: GL_OUT_OF_MEMORY";
			ASSERT(false);
			break;
		default:
			Log(MessageType::FAULT) << "ERROR: Unknown";
			ASSERT(false);
			break;
		}
	}
}

void Game::Render() {
	if (!m_Window.IsOpen())
		return;

	// Clear the window with grey.
	gl::ClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Draw game related stuff!
	EntityManagerInstance.InvokeSystems();

	// Switch the buffers.
	m_Window.GetWindow().display();
}