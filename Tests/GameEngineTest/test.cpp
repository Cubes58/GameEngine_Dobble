#include "pch.h"

#define private public
#define protected public

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Cursor.hpp>

#include "Window.h"
#include "Vector2D.h"			//
#include "FileSystemHelper.h"
#include "Collision.h"

#include "EntityManager.h"
#include "ResourceManager.h"
#include "AudioManager.h"
#include "ParticleManager.h"

#include "RenderSystem.h"

#include "RenderComponent.h"
#include "TransformComponent.h"

#include "Particle.h"

#include "Client.h"			//
#include "PacketTypes.h"

#include "Scene.h"
#include "GamePlayScene.h"

#include "Shape.h"			//
#include "CircleShape.h"	//
#include "RectangleShape.h"	//

#include "Button.h"
#include "FontRenderer.h"
#include "Text.h"
#include "UserInterface.h"

#include "Game.h"			//
#include "GameState.h"

#include "PostProcessor.h"	//
#include "Shader.h"			//
#include "Texture2D.h"		//

TEST(FileSystemHelperTest, GetFilesInFolder) {
	std::vector<FileInformation> filesInFolder = FileSystemHelper::GetFilesInFolder("resources/", true);
	bool success = false;
	if (filesInFolder.size() > 0) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(FileSystemHelperTest, RetainRemoveFilesWithExtensionsUsingStrings) {
	std::vector<FileInformation> filesInFolder = FileSystemHelper::GetFilesInFolder("resources/", true);
	std::vector<std::string> filesInFolderStrings;
	for (auto &file : filesInFolder) {
		filesInFolderStrings.emplace_back(file.m_Location);
	}
	
	bool success = false;
	unsigned int numberOfFilesFound = filesInFolderStrings.size();

	// Keep only selected files.
	FileSystemHelper::RetainRemoveFilesWithExtensions(filesInFolderStrings, { ".txt", ".png" }, true);

	if (filesInFolderStrings.size() < numberOfFilesFound) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(FileSystemHelperTest, RetainRemoveFilesWithExtensionsUsingFileInformationObjects) {
	std::vector<FileInformation> filesInFolder = FileSystemHelper::GetFilesInFolder("resources/", true);
	bool success = false;
	unsigned int numberOfFilesFound = filesInFolder.size();

	// Keep only selected files.
	FileSystemHelper::RetainRemoveFilesWithExtensions(filesInFolder, { ".txt", ".png" }, true);

	if (filesInFolder.size() < numberOfFilesFound) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(FileSystemHelperTest, RemoveFileExtension) {
	std::string file = "resources/audioFile.wav";
	EXPECT_NO_THROW(FileSystemHelper::RemoveFileExtension(file));

	bool success = false;
	if (file == "resources/audioFile") {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(FileSystemHelperTest, RemoveFilePath) {
	std::string file = "resources/audioFile.wav";
	EXPECT_NO_THROW(FileSystemHelper::RemoveFilePath(file));

	bool success = false;
	if (file == "audioFile.wav") {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(FileSystemHelperTest, GetNameFromFile) {
	std::string file = "resources/audioFile.wav";
	std::string fileName = "";
	EXPECT_NO_THROW(fileName = FileSystemHelper::GetNameFromFile(file));

	bool success = false;
	if (fileName == "audioFile") {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(FileSystemHelperTest, DoesLocationExist) {
	std::string invalidLocation = "resources/test/test/test/fake/audioFile.wav";
	bool doesInvalidLocationExist = true;
	EXPECT_NO_THROW(doesInvalidLocationExist = FileSystemHelper::DoesLocationExist(invalidLocation));

	std::string validLocation = "resources/";
	bool doesValidLocationExist = true;
	EXPECT_NO_THROW(doesValidLocationExist = FileSystemHelper::DoesLocationExist(validLocation));

	bool success = false;
	if (!doesInvalidLocationExist && doesValidLocationExist) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(WindowTest, Initialize) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;

	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));

	bool success = true;
	// Ensure all of the properties are the same, and that the window opens, when created.
	if (window->m_Width != (int)width || window->m_Height != (int)height 
		|| window->m_Style != sf::Style::Close || !window->m_Window.isOpen()) {
		success = false;
	}
	EXPECT_TRUE(success);
}

TEST(WindowTest, IsOpen) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;

	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));
	EXPECT_TRUE(window->IsOpen() && window->m_Window.isOpen());
}

TEST(WindowTest, Close) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;

	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));
	window->Close();
	EXPECT_FALSE(window->IsOpen() && window->m_Window.isOpen());
}

TEST(WindowTest, Display) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;

	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));
	EXPECT_NO_THROW(window->Display());
}

TEST(WindowTest, SetIcon) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;

	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));
	bool success = false;
	EXPECT_NO_THROW(success = window->SetIcon("resources/images/dobbleIcon.png"));

	EXPECT_TRUE(success);
}

TEST(WindowTest, SetScreenSize) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;

	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));
	bool success = false;

	unsigned int newWidth = 1920, newHeight = 1080;
	EXPECT_NO_THROW(window->SetScreenSize(newWidth, newHeight));

	if (window->m_Width == newWidth && window->m_Height == newHeight &&
		window->m_Window.m_size.x == newWidth && window->m_Window.m_size.y == newHeight) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(WindowTest, GetSFMLWindowInstance) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;

	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));

	// If there's no window to return it'll be null, which will throw an error.
	EXPECT_NO_THROW(window->GetWindow().close());
}

TEST(WindowTest, GetStyle) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;
	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));

	EXPECT_TRUE(window->m_Style == window->GetStyle());
}

TEST(WindowTest, GetWidth) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;
	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));

	EXPECT_TRUE(window->GetWidth() == window->m_Width && window->GetWidth() == window->GetWindow().getSize().x);
}

TEST(WindowTest, GetHeight) {
	float width = 1280.0f, height = 720.0f;
	std::shared_ptr<Window> window;
	EXPECT_NO_THROW(window = std::make_shared<Window>(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close));

	EXPECT_TRUE(window->GetHeight() == window->m_Height && window->GetHeight() == window->GetWindow().getSize().y);
}

TEST(RenderComponentTest, DefaultInitialization) {	
	EXPECT_NO_THROW(RenderComponent renderComponent;);
}

TEST(RenderComponentTest, NumberOfCirclesGivenInitialization) {
	unsigned int numberOfCirclesPerCard = 9;
	RenderComponent renderComponent(numberOfCirclesPerCard);

	EXPECT_TRUE(renderComponent.m_SymbolTextureIDs.size() == numberOfCirclesPerCard);
}

TEST(RenderComponentTest, DataInitialization) {
	unsigned int numberOfCirclesPerCard = 9;
	std::vector<unsigned int> symbolTextureIDs;
	symbolTextureIDs.reserve(numberOfCirclesPerCard);

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		symbolTextureIDs.emplace_back(i);
	}
	RenderComponent renderComponent(symbolTextureIDs);

	bool sameData = true;
	if (renderComponent.m_SymbolTextureIDs.size() != symbolTextureIDs.size()) {
		sameData = false;
	}

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		if (symbolTextureIDs[i] != renderComponent.m_SymbolTextureIDs[i]) {
			sameData = false;
		}
	}

	EXPECT_TRUE(sameData);
}

TEST(RenderComponentTest, DataChange) {
	unsigned int numberOfCirclesPerCard = 9;
	std::vector<unsigned int> symbolTextureIDs;
	symbolTextureIDs.reserve(numberOfCirclesPerCard);

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		symbolTextureIDs.emplace_back(i);
	}
	RenderComponent renderComponent(symbolTextureIDs);

	bool sameData = true;
	if (renderComponent.m_SymbolTextureIDs.size() != symbolTextureIDs.size()) {
		sameData = false;
	}

	renderComponent.m_SymbolTextureIDs[0] = 10;

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		if (symbolTextureIDs[i] != renderComponent.m_SymbolTextureIDs[i]) {
			sameData = false;
		}
	}

	EXPECT_TRUE(!sameData);
}

TEST(TransformComponentTest, DefaultInitialization) {
	EXPECT_NO_THROW(TransformComponent transformComponent);
}

TEST(TransformComponentTest, NumberOfCirclesGivenInitialization) {
	unsigned int numberOfCirclesPerCard = 9;
	TransformComponent transformComponent(numberOfCirclesPerCard);

	EXPECT_TRUE(transformComponent.m_CircleTransforms.size() == numberOfCirclesPerCard);
}

TEST(TransformComponentTest, DataInitialization) {
	unsigned int numberOfCirclesPerCard = 9;
	std::vector<CircleTransformData> circleTransformData;
	circleTransformData.reserve(numberOfCirclesPerCard);

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		circleTransformData.emplace_back(Vector2Df((float)i, (float)i), i * 10.0f, i * 15.0f);
	}
	TransformComponent transformComponent(circleTransformData);

	bool sameData = true;
	if (transformComponent.m_CircleTransforms.size() != circleTransformData.size()) {
		sameData = false;
	}

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		if (circleTransformData[i] != transformComponent.m_CircleTransforms[i]) {
			sameData = false;
		}
	}

	EXPECT_TRUE(sameData);
}

TEST(TransformComponentTest, DataChange) {
	unsigned int numberOfCirclesPerCard = 9;
	std::vector<CircleTransformData> circleTransformData;
	circleTransformData.reserve(numberOfCirclesPerCard);

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		circleTransformData.emplace_back(Vector2Df((float)i, (float)i), i * 10.0f, i * 15.0f);
	}
	TransformComponent transformComponent(circleTransformData);

	bool sameData = true;
	if (transformComponent.m_CircleTransforms.size() != circleTransformData.size()) {
		sameData = false;
	}

	circleTransformData[0].m_Position = Vector2Df(1.0f, 1000.0f);

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		if (circleTransformData[i] != transformComponent.m_CircleTransforms[i]) {
			sameData = false;
		}
	}

	EXPECT_TRUE(!sameData);
}

TEST(EntityManagerTest, AccessInstance) {
	EXPECT_NO_THROW(EntityManager::Instance());
}

TEST(EntityManagerTest, AccessInstanceUsingMacro) {
	EXPECT_NO_THROW(EntityManagerInstance);
}

TEST(EntityManagerTest, CreateComponentWithName) {
	std::hash<std::string> m_StringHasher;

	std::string name = "myEntity";
	EntityManagerInstance.CreateEntity(name);

	EntityID entityID = m_StringHasher(name);

	bool success = false;
	auto iter = EntityManagerInstance.GetEntities()->find(entityID);
	if (iter != EntityManagerInstance.GetEntities()->end())
		success = true;

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, CreateComponentWithEntityID) {
	EntityID entityID = 10;
	EntityManagerInstance.CreateEntity(entityID);

	bool success = false;
	auto iter = EntityManagerInstance.GetEntities()->find(entityID);
	if (iter != EntityManagerInstance.GetEntities()->end())
		success = true;

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, GetEntities) {
	EntityID entityID1 = 10;
	EntityManagerInstance.CreateEntity(entityID1);

	const std::set<EntityID> *entities = EntityManagerInstance.GetEntities();

	bool success = false;
	for (auto &entityID : *entities) {
		if (entityID == entityID1)
			success = true;
	}

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, AddAndGetEntityComponentUsingEntityID) {
	// Create an entity.
	EntityID entityID = 10;
	EntityManagerInstance.CreateEntity(entityID);

	// Add a component to it.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityID, std::make_shared<RenderComponent>(renderComponent));

	// Get the component.
	std::shared_ptr<RenderComponent> renderComponentPointer = EntityManagerInstance.GetComponent<RenderComponent>(entityID);

	// Verify that it's the correct component.
	bool success = true;
	for (int i = 0; i < numberOfCircles; ++i) {
		if (renderComponent.m_SymbolTextureIDs[i] != renderComponentPointer->m_SymbolTextureIDs[i]) {
			success = false;
		}
	}

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, AddAndGetEntityComponentUsingEntityName) {
	// Create an entity.
	std::string entityID = "MyEntity";
	EntityManagerInstance.CreateEntity(entityID);

	// Add a component to it.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityID, std::make_shared<RenderComponent>(renderComponent));

	// Get the component.
	std::shared_ptr<RenderComponent> renderComponentPointer = EntityManagerInstance.GetComponent<RenderComponent>(entityID);

	// Verify that it's the correct component.
	bool success = true;
	for (int i = 0; i < numberOfCircles; ++i) {
		if (renderComponent.m_SymbolTextureIDs[i] != renderComponentPointer->m_SymbolTextureIDs[i]) {
			success = false;
		}
	}

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, DeleteComponentUsingEntityID) {
	// Create entity.
	EntityID entityID = 10;
	EntityManagerInstance.CreateEntity(entityID);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityID, std::make_shared<RenderComponent>(renderComponent));

	bool success = false;
	if (EntityManagerInstance.GetComponent<RenderComponent>(entityID) != nullptr) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, DeleteComponentUsingEntityName) {
	// Create entity.
	std::string entityName = "MyEntity";
	EntityManagerInstance.CreateEntity(entityName);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityName, std::make_shared<RenderComponent>(renderComponent));

	bool success = false;
	if (EntityManagerInstance.GetComponent<RenderComponent>(entityName) != nullptr) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, DeleteEntityUsingEntityID) {
	EntityManagerInstance.Clear();

	// Create entity.
	int entityID = 10;
	EntityManagerInstance.CreateEntity(entityID);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityID, std::make_shared<RenderComponent>(renderComponent));
	
	EntityManagerInstance.DeleteEntity(entityID);
	
	bool success = false;
	if (EntityManagerInstance.GetEntities()->size() <= 0) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, DeleteEntityUsingEntityName) {
	EntityManagerInstance.Clear();

	// Create entity.
	std::string entityName = "MyEntity";
	EntityManagerInstance.CreateEntity(entityName);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityName, std::make_shared<RenderComponent>(renderComponent));

	EntityManagerInstance.DeleteEntity(entityName);

	bool success = false;
	if (EntityManagerInstance.GetEntities()->size() <= 0) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, AddEntitySystem) {
	EntityManagerInstance.Clear();
	EntityManagerInstance.Init();

	// Create entity.
	std::string entityName = "MyEntity";
	EntityManagerInstance.CreateEntity(entityName);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityName, std::make_shared<RenderComponent>(renderComponent));

	float width = 1280.0f, height = 720.0f;
	// Create an OpenGL context, so OpenGL methods can be used.
	Window window(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close);

	EXPECT_NO_FATAL_FAILURE(EntityManagerInstance.AddSystem(std::make_shared<RenderSystem>(width, height)));
}

TEST(EntityManagerTest, UpdateEntitySystem) {
	EntityManagerInstance.Clear();
	EntityManagerInstance.Init();

	// Create entity.
	std::string entityName = "MyEntity";
	EntityManagerInstance.CreateEntity(entityName);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityName, std::make_shared<RenderComponent>(renderComponent));

	float width = 1280.0f, height = 720.0f;
	// Create an OpenGL context, so OpenGL methods can be used.
	Window window(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close);

	EntityManagerInstance.AddSystem(std::make_shared<RenderSystem>(width, height));

	EXPECT_NO_FATAL_FAILURE(EntityManagerInstance.UpdateSystems(1.0f / 60.0f));
}

TEST(EntityManagerTest, RenderEntitySystem) {
	EntityManagerInstance.Clear();
	EntityManagerInstance.Init();

	// Create entity.
	std::string entityName = "MyEntity";
	EntityManagerInstance.CreateEntity(entityName);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityName, std::make_shared<RenderComponent>(renderComponent));

	float width = 1280.0f, height = 720.0f;
	// Create an OpenGL context, so OpenGL methods can be used.
	Window window(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close);

	EntityManagerInstance.AddSystem(std::make_shared<RenderSystem>(width, height));

	EXPECT_NO_FATAL_FAILURE(EntityManagerInstance.RenderSystems());
}

TEST(EntityManagerTest, ClearEntityManagerData) {
	EntityManagerInstance.Init();

	// Create entity.
	std::string entityName = "MyEntity";
	EntityManagerInstance.CreateEntity(entityName);

	// Create and add component.
	int numberOfCircles = 8;
	RenderComponent renderComponent;
	for (int i = 0; i < numberOfCircles; ++i) {
		renderComponent.m_SymbolTextureIDs.emplace_back(i);
	}
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(entityName, std::make_shared<RenderComponent>(renderComponent));

	float width = 1280.0f, height = 720.0f;
	// Create an OpenGL context, so OpenGL methods can be used.
	Window window(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close);
	// Add an entity system.
	EntityManagerInstance.AddSystem(std::make_shared<RenderSystem>(width, height));

	// Use Clear to remove all of the entities, components, and entity systems.
	EntityManagerInstance.Clear();

	bool success = true;
	if (EntityManagerInstance.m_Components.size() > 0)
		success = false;
	if (EntityManagerInstance.m_Entities.size() > 0)
		success = false;
	if (EntityManagerInstance.m_Systems.size() > 0)
		success = false;

	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, AccessInstance) {
	EXPECT_NO_THROW(AudioManager::Instance());
}

TEST(AudioManagerTest, AccessInstanceUsingMacro) {
	EXPECT_NO_THROW(AudioManagerInstance);
}

TEST(AudioManagerTest, OpenMusicFile) {
	bool success = false;

	EXPECT_NO_THROW(success = AudioManagerInstance.OpenMusicFile("resources/audio/music/dobbleTheme.wav", true));
	if (!AudioManagerInstance.m_BackgroundMusic.m_isStreaming)
		success = false;

	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, LoadSoundEffectFileWithNameAndFile) {
	bool success = false;

	std::string soundEffectName = "bleep";
	EXPECT_NO_THROW(success = AudioManagerInstance.LoadSoundEffect("bleep", "resources/audio/soundEffects/" + soundEffectName + ".wav"));

	// Ensure the sound effect has been loaded - find the loaded instance.
	auto iter = AudioManagerInstance.m_SoundEffects.find(soundEffectName);
	if (iter == AudioManagerInstance.m_SoundEffects.end()) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, LoadSoundEffectFileWithJustFile) {
	bool success = false;

	std::string soundEffectName = "symbolGuess";
	EXPECT_NO_THROW(success = AudioManagerInstance.LoadSoundEffect("resources/audio/soundEffects/" + soundEffectName + ".wav"));

	auto iter = AudioManagerInstance.m_SoundEffects.find(soundEffectName);
	if (iter == AudioManagerInstance.m_SoundEffects.end()) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, LoadSoundEffectsFromFolder) {
	bool success = false;
	// Load files, even if there aren't any to load there shouldn't be an exception thrown!
	EXPECT_NO_THROW(success = AudioManagerInstance.LoadSoundEffects("resources/audio/soundEffects/"));

	// Ensure the files were loaded.
	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, PlaySoundEffect) {
	bool success = false;

	std::string soundEffectName = "bleep";
	EXPECT_NO_THROW(AudioManagerInstance.LoadSoundEffect("bleep", "resources/audio/soundEffects/" + soundEffectName + ".wav"));
	EXPECT_NO_THROW(success = AudioManagerInstance.PlaySoundEffect(soundEffectName));

	// Ensure the files were loaded.
	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, GetSoundEffect) {
	bool success = true;

	std::string soundEffectName = "bleep";
	EXPECT_NO_THROW(AudioManagerInstance.LoadSoundEffect("bleep", "resources/audio/soundEffects/" + soundEffectName + ".wav"));

	const sf::SoundBuffer *soundBufferPointer = nullptr;
	EXPECT_NO_THROW(soundBufferPointer = AudioManagerInstance.GetSoundEffect(soundEffectName));

	if (soundBufferPointer == nullptr)
		success = false;

	// Ensure the files were loaded.
	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, GetBackgroundMusic) {
	bool success = false;

	EXPECT_NO_THROW(AudioManagerInstance.OpenMusicFile("resources/audio/music/dobbleTheme.wav", true));

	AudioManagerInstance.GetBackgroundMusic().stop();
	if (AudioManagerInstance.m_BackgroundMusic.getStatus() == sf::Music::Stopped) {
		success = true;
	}

	// Ensure the files were loaded.
	EXPECT_TRUE(success);
}

TEST(ParticleManagerTest, InitializeData) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::shared_ptr<ParticleManager> particleManager;
	EXPECT_NO_THROW(particleManager = std::make_shared<ParticleManager>(screenSize, 50));

	bool success = true;
	if (particleManager->m_NumberOfParticles != 50)
		success = false;

	EXPECT_TRUE(success);
}

TEST(ParticleManagerTest, UpdateParticles) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	ParticleManager particleManager(screenSize, 50);

	bool success = true;
	particleManager.Update(1.0f / 60.0f, Vector2Df(0.0f, 0.0f));
	
	for (unsigned int i = 0; i < particleManager.m_NumberOfParticlesToSpawnEachTime; i++) {
		if (particleManager.m_Particles[i].m_Life == particleManager.m_ParticleLifeDuration) {
			success = true;
		}
	}
	if (particleManager.m_Particles[particleManager.m_NumberOfParticlesToSpawnEachTime].m_Life > 0.0f) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(ParticleManagerTest, RenderParticles) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	ParticleManager particleManager(screenSize, 50);

	bool success = true;
	particleManager.Update(1.0f / 60.0f, Vector2Df(0.0f, 0.0f));
	EXPECT_NO_THROW(particleManager.Render());

	EXPECT_TRUE(success);
}

TEST(ParticleManagerTest, NumberOfParticlesToSpawnGetter) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	ParticleManager particleManager(screenSize, 50);

	bool success = true;
	particleManager.m_NumberOfParticlesToSpawnEachTime == particleManager.GetNumberOfParticlesToSpawnEachTime() ? success = true : success = false;

	EXPECT_TRUE(success);
}

TEST(ParticleManagerTest, NumberOfParticlesToSpawnSetter) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	ParticleManager particleManager(screenSize, 500);

	unsigned int originalNumberOfParticlesToSpawn = particleManager.m_NumberOfParticlesToSpawnEachTime;
	unsigned int newNumberOfParticlesToSpawn = 10;

	particleManager.SetNumberOfParticlesToSpawnEachTime(newNumberOfParticlesToSpawn);

	bool success = true;
	particleManager.m_NumberOfParticlesToSpawnEachTime == newNumberOfParticlesToSpawn ? success = true : success = false;

	EXPECT_TRUE(success);
}

TEST(MouseInputTest, MousePosition) {
	sf::Mouse mouseEvent;
	sf::Vector2i givenPosition = sf::Vector2i(10, 10);
	mouseEvent.setPosition(givenPosition);

	sf::Vector2i mousePosition;
	mousePosition = mouseEvent.getPosition();

	EXPECT_TRUE(givenPosition == mousePosition);
}

TEST(MouseInputTest, SymbolGuess) {
	sf::Mouse mouseEvent;
	sf::Vector2i givenPosition = sf::Vector2i(10, 10);
	mouseEvent.setPosition(givenPosition);
	sf::Vector2i mousePosition;
	mousePosition = mouseEvent.getPosition();

	unsigned int numberOfCirclesPerCard = 9;
	std::vector<CircleTransformData> circleTransformData;
	circleTransformData.reserve(numberOfCirclesPerCard);

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		circleTransformData.emplace_back(Vector2Df((float)i, (float)i), i * 10.0f, i * 15.0f);
	}
	TransformComponent transformComponent(circleTransformData);
	transformComponent.m_CircleTransforms[0].m_Position = Vector2Df(10.0f, 10.0f);
	transformComponent.m_CircleTransforms[0].m_Radius = 50.0f;

	bool success = false;
	Collision collisionChecker;
	if (collisionChecker(transformComponent.m_CircleTransforms[0].m_Position, transformComponent.m_CircleTransforms[0].m_Radius, Vector2Df(mousePosition.x, mousePosition.y)))
		success = true;

	EXPECT_TRUE(success);
}

TEST(ResourceManagerTest, AccessInstance) {
	EXPECT_NO_THROW(ResourceManager::Instance());
}

TEST(ResourceManagerTest, AccessInstanceUsingMacro) {
	EXPECT_NO_THROW(ResourceManagerInstance);
}

TEST(ResourceManagerTest, LoadShadersFromFolder) {
	bool success = false;
	EXPECT_NO_THROW(success = ResourceManagerInstance.LoadShadersFromFolder("resources/shaders/"));

	EXPECT_TRUE(success);
}

TEST(ResourceManagerTest, LoadShader) {
	bool success = false;
	Shader *shader = nullptr;

	EXPECT_NO_THROW(shader = ResourceManagerInstance.LoadShader("resources/shaders/TestShader.vert", "resources/shaders/TestShader.frag"));

	if (ResourceManagerInstance.m_Shaders.size() > 0 && shader != nullptr) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(ResourceManagerTest, GetShader) {
	bool success = false;
	Shader *shader = nullptr;
	Shader *getterShader = nullptr;

	EXPECT_NO_THROW(shader = ResourceManagerInstance.LoadShader("resources/shaders/TestShader.vert", "resources/shaders/TestShader.frag"));

	EXPECT_NO_THROW(getterShader = ResourceManagerInstance.GetShader("TestShader"));

	if (ResourceManagerInstance.m_Shaders.size() > 0 && shader != nullptr && getterShader != nullptr) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(ResourceManagerTest, LoadTexturesFromFolder) {
	bool success = false;

	EXPECT_NO_THROW(success = ResourceManagerInstance.LoadTexturesFromFolder("resources/images/"));

	EXPECT_TRUE(success);
}

TEST(ResourceManagerTest, LoadTextureFromFile) {
	bool success = false;
	Texture2D *texture = nullptr;

	EXPECT_NO_THROW(texture = ResourceManagerInstance.LoadTexture("resources/images/default.png"));

	if (texture != nullptr && ResourceManagerInstance.m_Textures.size() > 0) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(ResourceManagerTest, GetTextureUsingName) {
	// Load the texture in.
	EXPECT_NO_THROW(ResourceManagerInstance.LoadTexture("resources/images/default.png"));

	bool success = false;
	Texture2D *texture = nullptr;

	// Get the texture.
	EXPECT_NO_THROW(texture = ResourceManagerInstance.GetTexture("default"));

	if (texture != nullptr) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(ResourceManagerTest, GetTextureUsingID) {
	// Load the texture in.
	EXPECT_NO_THROW(ResourceManagerInstance.LoadTexture("resources/images/default.png"));

	bool success = false;
	Texture2D *texture = nullptr;

	// Get the texture.
	EXPECT_NO_THROW(texture = ResourceManagerInstance.GetTexture(0));

	if (texture != nullptr) {
		success = true;
	}

	EXPECT_TRUE(success);
}

TEST(GameStateEnumeratorTest, EnumeratorCreation) {
	EXPECT_NO_THROW(GameState gameState = GameState::MAIN_MENU);
}

TEST(ButtonTest, InitializeData) {
	std::shared_ptr<Button> button = nullptr;
	std::shared_ptr<Shape> shape = std::make_shared<CircleShape>();

	EXPECT_NO_THROW(button = std::make_shared<Button>(shape, typeid(CircleShape)));
}

TEST(ButtonTest, InitializeDataWithStateChange) {
	std::shared_ptr<Button> button = nullptr;
	std::shared_ptr<Shape> shape = std::make_shared<CircleShape>();

	EXPECT_NO_THROW(button = std::make_shared<Button>(shape, typeid(CircleShape), GameState::MAIN_MENU));

	bool success = true;
	if (button->m_GameState != GameState::MAIN_MENU || button->m_ShapeType != typeid(CircleShape)) {
		success = false;
	}
	EXPECT_TRUE(success);
}

TEST(TextTest, InitializeData) {
	std::string textString = "Test The Text!";
	Vector2Df position(0.0f, 0.0f);
	float size = 0.6f;
	glm::vec3 colour = glm::vec3(0.1f, 0.1f, 0.1f);

	Text text(textString, position, size, colour);

	bool success = true;
	if (text.m_Text != textString || text.m_Position != position || text.m_Size != size || text.m_Colour != colour) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(ReferenceTextTest, InitializeData) {
	std::string textString = "Test The Text!";
	Vector2Df position(0.0f, 0.0f);
	float size = 0.6f;
	glm::vec3 colour = glm::vec3(0.1f, 0.1f, 0.1f);


	int referenceData = 5;
	ReferenceText<int> text(textString, referenceData, position, size, colour);

	bool success = true;
	if (text.m_Text != textString || text.m_Position != position || text.m_Size != size || text.m_Colour != colour) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(ReferenceTextTest, UpdateChangedReferenceData) {
	std::string textString = "Test The Text!";
	Vector2Df position(0.0f, 0.0f);
	float size = 0.6f;
	glm::vec3 colour = glm::vec3(0.1f, 0.1f, 0.1f);


	int referenceData = 5;
	ReferenceText<int> text(textString, referenceData, position, size, colour);
	bool success = true;

	referenceData = 10;
	text.Update();

	if (text.m_ReferenceData != referenceData) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(ParticleTest, InitializeData) {
	EXPECT_NO_THROW(Particle particle);
}

TEST(PacketTypesTest, EnumeratorCreation) {
	EXPECT_NO_THROW(Packet::PacketType packetType = Packet::CONNECT);
}

TEST(PacketTypesTest, GetPacketType) {
	sf::Packet packet;
	packet << PacketID(Packet::CONNECT);

	bool success = true;
	if (Packet::GetPacketType(packet) != Packet::CONNECT) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(PacketTypesTest, SetPacketTypeExistingPacket) {
	sf::Packet packet;
	Packet::SetPacketType(Packet::CONNECT, packet);

	PacketID packetID;
	packet >> packetID;

	bool success = true;
	if (packetID != Packet::CONNECT) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(PacketTypesTest, SetPacketTypeOfNewPacket) {
	sf::Packet packet = Packet::SetPacketType(Packet::CONNECT);

	PacketID packetID;
	packet >> packetID;

	bool success = true;
	if (packetID != Packet::CONNECT) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(GameTest, Initialize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	//Game game(window);

	bool success = false;

	EXPECT_TRUE(success);
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}