#include "Game.h"

#include "GLCore.hpp"

#include "EntityManager.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "MoveSymbolsSystem.h"
#include "Logger.h"
#include "Scene.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"

Game::Game(Window &p_Window) : m_Window(p_Window), m_GameState(GameState::GAME_ACTIVE) {
	for (int i = 0; i < m_s_NumberOfDifferentKeyCodes; ++i) {
		m_Keys[i] = false;
	}

	EntityManagerInstance.Init();

	// Entity One:
	EntityManager::Instance().CreateEntity("EntityOne"); 
	TransformComponent positionComponent;

	int width = 1280;
	int height = 720;
	positionComponent.m_CircleTransforms[0].m_Position = Vector2D<float>(200.0f, 200.0f);
	positionComponent.m_CircleTransforms[0].m_Radius = 200;
	positionComponent.m_CircleTransforms[0].m_Rotation = 0;

	positionComponent.m_CircleTransforms[1].m_Position = Vector2D<float>(150.0f, 150.0f);
	positionComponent.m_CircleTransforms[1].m_Radius = 40;
	positionComponent.m_CircleTransforms[1].m_Rotation = 0;
	
	positionComponent.m_CircleTransforms[2].m_Position = Vector2D<float>(225.0f, 200.0f);
	positionComponent.m_CircleTransforms[2].m_Radius = 25;
	positionComponent.m_CircleTransforms[2].m_Rotation = 30;
	
	positionComponent.m_CircleTransforms[3].m_Position = Vector2D<float>(100.0f, 100.0f);
	positionComponent.m_CircleTransforms[3].m_Radius = 25.75;
	positionComponent.m_CircleTransforms[3].m_Rotation = 20;
	
	positionComponent.m_CircleTransforms[4].m_Position = Vector2D<float>(290.0f, 110.0f);
	positionComponent.m_CircleTransforms[4].m_Radius = 55;
	positionComponent.m_CircleTransforms[4].m_Rotation = 70;
	
	positionComponent.m_CircleTransforms[5].m_Position = Vector2D<float>(200.0f, 50.0f);
	positionComponent.m_CircleTransforms[5].m_Radius = 27.5;
	positionComponent.m_CircleTransforms[5].m_Rotation = 90;
	
	positionComponent.m_CircleTransforms[6].m_Position = Vector2D<float>(150.0f, 300.0f);
	positionComponent.m_CircleTransforms[6].m_Radius = 44;
	positionComponent.m_CircleTransforms[6].m_Rotation = 45;
	
	positionComponent.m_CircleTransforms[7].m_Position = Vector2D<float>(45.0f, 200.0f);
	positionComponent.m_CircleTransforms[7].m_Radius = 35;
	positionComponent.m_CircleTransforms[7].m_Rotation = 367;
	
	positionComponent.m_CircleTransforms[8].m_Position = Vector2D<float>(280.0f, 310.0f);
	positionComponent.m_CircleTransforms[8].m_Radius = 53;
	positionComponent.m_CircleTransforms[8].m_Rotation = 98;

	EntityManager::Instance().AddComponentToEntity<TransformComponent>("EntityOne", std::make_shared<TransformComponent>(positionComponent));
	
	RenderComponent renderComponent;
	renderComponent.m_SymbolTextureNames[0] = ResourceManager::Instance().LoadTexture("resources/images/cardBackground.png");
	renderComponent.m_SymbolTextureNames[1] = ResourceManager::Instance().LoadTexture("resources/images/symbols/airplane.png");
	renderComponent.m_SymbolTextureNames[2] = ResourceManager::Instance().LoadTexture("resources/images/symbols/apple.png");
	renderComponent.m_SymbolTextureNames[3] = ResourceManager::Instance().LoadTexture("resources/images/symbols/appleTree.png");
	renderComponent.m_SymbolTextureNames[4] = ResourceManager::Instance().LoadTexture("resources/images/symbols/bear.png");
	renderComponent.m_SymbolTextureNames[5] = ResourceManager::Instance().LoadTexture("resources/images/symbols/binoculars.png");
	renderComponent.m_SymbolTextureNames[6] = ResourceManager::Instance().LoadTexture("resources/images/symbols/blackCat.png");
	renderComponent.m_SymbolTextureNames[7] = ResourceManager::Instance().LoadTexture("resources/images/symbols/boar.png");
	renderComponent.m_SymbolTextureNames[8] = ResourceManager::Instance().LoadTexture("resources/images/symbols/bonsai.png");
	EntityManager::Instance().AddComponentToEntity<RenderComponent>("EntityOne", std::make_shared<RenderComponent>(renderComponent));

	CollisionComponent collisionComponent;
	EntityManager::Instance().AddComponentToEntity<CollisionComponent>("EntityOne", std::make_shared<CollisionComponent>(collisionComponent));

	// Entity Two:
	EntityManager::Instance().CreateEntity("EntityTwo");
	TransformComponent positionComponentTwo;

	auto var = 400.0f;
	positionComponentTwo.m_CircleTransforms[0].m_Position = Vector2D<float>(width - (200.0f + var), 200.0f);
	positionComponentTwo.m_CircleTransforms[0].m_Radius = 200;
	positionComponentTwo.m_CircleTransforms[0].m_Rotation = 0;

	positionComponentTwo.m_CircleTransforms[1].m_Position = Vector2D<float>(width - (150.0f + var), 150.0f);
	positionComponentTwo.m_CircleTransforms[1].m_Radius = 40;				
	positionComponentTwo.m_CircleTransforms[1].m_Rotation = 0;				
																			
	//positionComponentTwo.m_CircleTransforms[2].m_Position = Vector2D<float>(width - (425.0f + var), 200.0f);	// LEFT CHECK
	//positionComponentTwo.m_CircleTransforms[2].m_Position = Vector2D<float>(width - (-100.0f + var), 200.0f);	// RIGHT CHECK
	//positionComponentTwo.m_CircleTransforms[2].m_Position = Vector2D<float>(width - (125.0f + var), 455.0f);	// BOTTOM CHECK
	//positionComponentTwo.m_CircleTransforms[2].m_Position = Vector2D<float>(width - (125.0f + var), -250.0f);	// TOP CHECK
	positionComponentTwo.m_CircleTransforms[2].m_Position = Vector2D<float>(width - (405.0f + var), 550.0f);	// RANDOM CHECK
	positionComponentTwo.m_CircleTransforms[2].m_Radius = 35;				
	positionComponentTwo.m_CircleTransforms[2].m_Rotation = 30;				
																			
	positionComponentTwo.m_CircleTransforms[3].m_Position = Vector2D<float>(width - (100.0f + var), 100.0f);
	positionComponentTwo.m_CircleTransforms[3].m_Radius = 25.75;			
	positionComponentTwo.m_CircleTransforms[3].m_Rotation = 20;				
																			
	positionComponentTwo.m_CircleTransforms[4].m_Position = Vector2D<float>(width - (290.0f + var), 110.0f);
	positionComponentTwo.m_CircleTransforms[4].m_Radius = 55;				
	positionComponentTwo.m_CircleTransforms[4].m_Rotation = 70;				
																			
	positionComponentTwo.m_CircleTransforms[5].m_Position = Vector2D<float>(width - (200.0f + var), 50.0f);
	positionComponentTwo.m_CircleTransforms[5].m_Radius = 27.5;				
	positionComponentTwo.m_CircleTransforms[5].m_Rotation = 90;				
																			
	positionComponentTwo.m_CircleTransforms[6].m_Position = Vector2D<float>(width - (150.0f + var), 300.0f);
	positionComponentTwo.m_CircleTransforms[6].m_Radius = 44;				
	positionComponentTwo.m_CircleTransforms[6].m_Rotation = 45;				
																			
	positionComponentTwo.m_CircleTransforms[7].m_Position = Vector2D<float>(width - (45.0f + var), 200.0f);
	positionComponentTwo.m_CircleTransforms[7].m_Radius = 35;				 
	positionComponentTwo.m_CircleTransforms[7].m_Rotation = 367;			 
																			 
	positionComponentTwo.m_CircleTransforms[8].m_Position = Vector2D<float>(width - (280.0f + var), 310.0f);
	positionComponentTwo.m_CircleTransforms[8].m_Radius = 53;
	positionComponentTwo.m_CircleTransforms[8].m_Rotation = 98;

	EntityManager::Instance().AddComponentToEntity<TransformComponent>("EntityTwo", std::make_shared<TransformComponent>(positionComponentTwo));

	RenderComponent renderComponentTwo;
	renderComponentTwo.m_SymbolTextureNames[0] = ResourceManager::Instance().LoadTexture("resources/images/cardBackground.png");
	renderComponentTwo.m_SymbolTextureNames[1] = ResourceManager::Instance().LoadTexture("resources/images/symbols/airplane.png");
	renderComponentTwo.m_SymbolTextureNames[2] = ResourceManager::Instance().LoadTexture("resources/images/symbols/apple.png");
	renderComponentTwo.m_SymbolTextureNames[3] = ResourceManager::Instance().LoadTexture("resources/images/symbols/appleTree.png");
	renderComponentTwo.m_SymbolTextureNames[4] = ResourceManager::Instance().LoadTexture("resources/images/symbols/bear.png");
	renderComponentTwo.m_SymbolTextureNames[5] = ResourceManager::Instance().LoadTexture("resources/images/symbols/binoculars.png");
	renderComponentTwo.m_SymbolTextureNames[6] = ResourceManager::Instance().LoadTexture("resources/images/symbols/blackCat.png");
	renderComponentTwo.m_SymbolTextureNames[7] = ResourceManager::Instance().LoadTexture("resources/images/symbols/boar.png");
	renderComponentTwo.m_SymbolTextureNames[8] = ResourceManager::Instance().LoadTexture("resources/images/symbols/bonsai.png");
	EntityManager::Instance().AddComponentToEntity<RenderComponent>("EntityTwo", std::make_shared<RenderComponent>(renderComponentTwo));
	EntityManager::Instance().AddComponentToEntity<CollisionComponent>("EntityTwo", std::make_shared<CollisionComponent>(collisionComponent));


	std::shared_ptr<RenderSystem> renderSystem = std::make_shared<RenderSystem>((float)m_Window.GetWidth(), (float)m_Window.GetHeight());
	EntityManagerInstance.AddSystem(renderSystem);

	std::shared_ptr<CollisionSystem> collisionSystem = std::make_shared<CollisionSystem>();
	EntityManagerInstance.AddSystem(collisionSystem);

	std::shared_ptr<MoveSymbolsSystem> moveSymbolsSystem = std::make_shared<MoveSymbolsSystem>();
	EntityManagerInstance.AddSystem(moveSymbolsSystem);
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
	EntityManagerInstance.UpdateSystems(p_DeltaTime);

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
			Log(MessageType::FAULT) << "ERROR: UNKNOWN";
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
	EntityManagerInstance.RenderSystems(m_Window);

	// Switch the buffers.
	m_Window.GetWindow().display();
}