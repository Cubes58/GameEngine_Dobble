#include "pch.h"

#define private public
#define protected public

#include <memory>
#include <vector>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Cursor.hpp>

#include "stb_image/stb_image.h"

#include "Window.h"
#include "Vector2D.h"
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

#include "Client.h"
#include "PacketTypes.h"

#include "Scene.h"
#include "GamePlayScene.h"

#include "Shape.h"
#include "CircleShape.h"
#include "RectangleShape.h"

#include "Button.h"
#include "FontRenderer.h"
#include "Text.h"
#include "UserInterface.h"

#include "Game.h"
#include "GameState.h"

#include "PostProcessor.h"
#include "Shader.h"
#include "Texture2D.h"

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
	float width = 1280.0f, height = 720.0f;
	// Create an OpenGL context, so OpenGL methods can be used.
	Window window(sf::VideoMode((int)width, (int)height), "Dobble", sf::Style::Close);

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

TEST(ClientTest, Initialize) {
	std::shared_ptr<Client> client;

	EXPECT_NO_THROW(client = std::make_shared<Client>());

	EXPECT_TRUE(client->m_ServerIPAddress.toString() != "0.0.0.0");
}

TEST(ClientTest, Connect) {
	std::shared_ptr<Client> client;

	EXPECT_NO_THROW(client = std::make_shared<Client>());

	bool success = false;
	EXPECT_NO_THROW(success = client->Connect());

	// As there won't be a server instance running.
	EXPECT_FALSE(success);
}

TEST(ClientTest, Disconnect) {
	std::shared_ptr<Client> client;

	EXPECT_NO_THROW(client = std::make_shared<Client>());
	EXPECT_NO_THROW(client->Disconnect());

	EXPECT_FALSE(client->m_Connected);
}

TEST(ClientTest, Send) {
	std::shared_ptr<Client> client;

	EXPECT_NO_THROW(client = std::make_shared<Client>());

	bool success = false;
	client->Connect();

	sf::Packet packet;
	EXPECT_NO_THROW(success = client->Send(packet));

	EXPECT_FALSE(success);
}

TEST(ClientTest, ReceiveData) {
	std::shared_ptr<Client> client;

	EXPECT_NO_THROW(client = std::make_shared<Client>());

	bool success = false;
	client->Connect();

	sf::Packet packet;
	// No data will be sent, so it should return instantly with false, and not wait for data to be received.
	EXPECT_NO_THROW(success = client->ReceiveData(packet));

	EXPECT_FALSE(success);
}

TEST(ClientTest, IsConnected) {
	std::shared_ptr<Client> client = std::make_shared<Client>();

	EXPECT_TRUE(client->m_Connected == client->IsConnected());
}

TEST(ClientTest, SetConnected) {
	std::shared_ptr<Client> client = std::make_shared<Client>();

	bool newConnectedState = true;
	EXPECT_NO_THROW(client->SetConnected(newConnectedState));

	EXPECT_TRUE(client->m_Connected == newConnectedState);
}

TEST(PostProcessingTest, Initialize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	EXPECT_NO_THROW(PostProcessor postProcessor(screenSize));
}

TEST(PostProcessingTest, Update) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	float updateDeltaTime = 1.0f / 60.0f;
	PostProcessor postProcessor(screenSize);

	EXPECT_NO_THROW(postProcessor.Update(updateDeltaTime));

	bool success = true;

	if (postProcessor.m_AccumulatedTime != updateDeltaTime ||
		postProcessor.m_TimePassedSinceOtherEffectActive != updateDeltaTime ||
		postProcessor.m_TimePassedSinceShakeActive != updateDeltaTime)
		success = false;

	EXPECT_TRUE(success);
}

TEST(PostProcessingTest, BeginRender) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	EXPECT_NO_THROW(postProcessor.BeginRender());
}

TEST(PostProcessingTest, Render) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	EXPECT_NO_THROW(postProcessor.Render());
}

TEST(PostProcessingTest, EndRender) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	EXPECT_NO_THROW(postProcessor.EndRender());
}

TEST(PostProcessingTest, SetShakeTime) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	float shakeTime = 50.0f;
	EXPECT_NO_THROW(postProcessor.SetShakeTime(shakeTime));

	EXPECT_TRUE(postProcessor.m_ShakeTime == shakeTime);
}

TEST(PostProcessingTest, SetShakeState) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	bool shakeState = true;
	EXPECT_NO_THROW(postProcessor.SetShakeState(shakeState));

	EXPECT_TRUE(postProcessor.m_Shake == shakeState);
}

TEST(PostProcessingTest, GetInvertColoursState) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	bool invertColoursState = true;
	postProcessor.m_InvertColours = invertColoursState;

	EXPECT_TRUE(postProcessor.GetInvertColoursState() == invertColoursState);
}

TEST(PostProcessingTest, SetInvertColoursState) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	bool invertColoursState = true;
	EXPECT_NO_THROW(postProcessor.SetInvertColoursState(invertColoursState));

	EXPECT_TRUE(postProcessor.m_InvertColours == invertColoursState);
}

TEST(PostProcessingTest, GetChaosState) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	bool chaosState = true;
	postProcessor.m_Chaos = chaosState;

	EXPECT_TRUE(postProcessor.GetChaosState() == chaosState);
}

TEST(PostProcessingTest, SetChaosState) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	bool chaosState = true;
	postProcessor.SetChaosState(chaosState);

	EXPECT_TRUE(postProcessor.m_Chaos == chaosState);
}

TEST(PostProcessingTest, SetOtherEffectTime) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	float otherEffectTime = 50.5f;
	EXPECT_NO_THROW(postProcessor.SetOtherEffectTime(otherEffectTime));

	EXPECT_TRUE(postProcessor.m_OtherEffectTime == otherEffectTime);
}

TEST(PostProcessingTest, SetTextureSize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	PostProcessor postProcessor(screenSize);

	Vector2Df newTextureSize(50.0f, 50.0f);
	postProcessor.SetTextureSize(newTextureSize);

	EXPECT_TRUE(postProcessor.m_TextureSize == newTextureSize);
}

TEST(ShaderTest, Initialize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::shared_ptr<Shader> shader;
	EXPECT_NO_THROW(shader = std::make_shared<Shader>());
	unsigned int invalidShaderID = shader->m_ID;

	// This is supposed to take shader code, but due to us not actually using it, to render stuff, it's fine.
	shader->Compile("           ", "           ");

	EXPECT_TRUE(shader->m_ID != invalidShaderID);
}

TEST(ShaderTest, SetUniforms) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::shared_ptr<Shader> shader;
	EXPECT_NO_THROW(shader = std::make_shared<Shader>());
	shader->Compile("           ", "           ");

	EXPECT_NO_THROW(shader->Use());

	EXPECT_NO_THROW(shader->SetBool("myBool", true));
	EXPECT_NO_THROW(shader->SetInt("myInt", 5));
	EXPECT_NO_THROW(shader->SetVec2("myVecTwo", glm::vec2(10.0f, 10.0f)));
	EXPECT_NO_THROW(shader->SetVec3("myVecThree", glm::vec3(10.0f, 10.0f, 10.0f)));
	EXPECT_NO_THROW(shader->SetVec4("myVecFour", glm::vec4(10.0f, 10.0f, 10.0f, 10.0f)));
	EXPECT_NO_THROW(shader->SetFloat("myFloat", 10.0f));
}

TEST(ShaderTest, GetID) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::shared_ptr<Shader> shader;
	EXPECT_NO_THROW(shader = std::make_shared<Shader>());
	shader->Compile("           ", "           ");

	EXPECT_TRUE(shader->GetID() == shader->m_ID);
}

TEST(ShaderTest, SetID) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::shared_ptr<Shader> shader;
	unsigned int newID = 15;
	EXPECT_NO_THROW(shader = std::make_shared<Shader>());
	shader->Compile("           ", "           ");
	shader->SetID(newID);

	EXPECT_TRUE(shader->m_ID == newID);
}

TEST(Texture2DTest, Initialize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	// Create a Texture object.
	Texture2D texture;
	bool success = true;

	// Load image data.
	int width = 0;
	int height = 0;
	int nrComponents = 0;

	unsigned char *image = stbi_load("resources/images/default.png", &width, &height, &nrComponents, 0);
	if (image) {
		if (nrComponents == 1) {
			texture.SetInternalFormat(gl::RED);
			texture.SetImageFormat(gl::RED);
		}
		else if (nrComponents == 3) {
			texture.SetInternalFormat(gl::RGB);
			texture.SetImageFormat(gl::RGB);
		}
		else if (nrComponents == 4) {
			texture.SetInternalFormat(gl::RGBA);
			texture.SetImageFormat(gl::RGBA);
		}
	}
	else
		success = false;

	texture.Generate(width, height, image);
	stbi_image_free(image);

	if (texture.m_Height != height || texture.m_Width != width)
		success = false;

	EXPECT_TRUE(success);
}

TEST(Texture2DTest, GetID) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_ID = 10;

	EXPECT_TRUE(texture.m_ID == texture.GetID());
}

TEST(Texture2DTest, SetID) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;

	GLuint newTextureID = 120;
	texture.SetID(newTextureID);

	EXPECT_TRUE(texture.m_ID == newTextureID);
}

TEST(Texture2DTest, Bind) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	bool success = true;

	int width = 0;
	int height = 0;
	int nrComponents = 0;

	unsigned char *image = stbi_load("resources/images/default.png", &width, &height, &nrComponents, 0);
	if (image) {
		if (nrComponents == 1) {
			texture.SetInternalFormat(gl::RED);
			texture.SetImageFormat(gl::RED);
		}
		else if (nrComponents == 3) {
			texture.SetInternalFormat(gl::RGB);
			texture.SetImageFormat(gl::RGB);
		}
		else if (nrComponents == 4) {
			texture.SetInternalFormat(gl::RGBA);
			texture.SetImageFormat(gl::RGBA);
		}
	}
	else
		success = false;

	texture.Generate(width, height, image);
	stbi_image_free(image);

	if (texture.m_Height != height || texture.m_Width != width)
		success = false;

	EXPECT_NO_THROW(texture.Bind());

	EXPECT_TRUE(success);
}

TEST(Texture2DTest, GetWidth) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_Width = 500;

	EXPECT_TRUE(texture.m_Width == texture.GetWidth());
}

TEST(Texture2DTest, GetHeight) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_Height = 500;

	EXPECT_TRUE(texture.m_Height == texture.GetHeight());
}

TEST(Texture2DTest, GetInternalFormat) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_InternalFormat = gl::RGB;

	EXPECT_TRUE(texture.m_InternalFormat == texture.GetInternalFormat());
}

TEST(Texture2DTest, SetInternalFormat) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;

	GLuint newInternalFormat = gl::BGR;
	texture.SetInternalFormat(newInternalFormat);

	EXPECT_TRUE(texture.m_InternalFormat == newInternalFormat);
}

TEST(Texture2DTest, GetImageFormat) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_ImageFormat = gl::RGBA;

	EXPECT_TRUE(texture.m_ImageFormat == texture.GetImageFormat());
}

TEST(Texture2DTest, SetImageFormat) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;

	GLuint newImageFormat = gl::RGBA;
	texture.SetImageFormat(newImageFormat);

	EXPECT_TRUE(texture.m_ImageFormat == newImageFormat);
}

TEST(Texture2DTest, GetWrapS) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_WrapS = gl::REPEAT;

	EXPECT_TRUE(texture.m_WrapS == texture.GetWrapS());
}

TEST(Texture2DTest, GetWrapT) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_WrapT = gl::REPEAT;

	EXPECT_TRUE(texture.m_WrapT == texture.GetWrapT());
}

TEST(Texture2DTest, GetFilterMin) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_FilterMin = gl::LINEAR;

	EXPECT_TRUE(texture.m_FilterMin == texture.GetFilterMin());
}

TEST(Texture2DTest, GetFilterMax) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Texture2D texture;
	texture.m_FilterMax = gl::LINEAR;

	EXPECT_TRUE(texture.m_FilterMax == texture.GetFilterMax());
}

TEST(RectangleShapeTest, Initialize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	Vector2Df size(50.0f, 50.0f);

	RectangleShape rectangleShape(position, size);

	EXPECT_TRUE(rectangleShape.m_Size == size && rectangleShape.m_Position == position);
}

TEST(RectangleShapeTest, Render) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	Vector2Df size(50.0f, 50.0f);

	RectangleShape rectangleShape(position, size);

	Shader &shader = *ResourceManagerInstance.GetShader("ShapesShader");
	EXPECT_NO_THROW(rectangleShape.Render(shader));
}

TEST(RectangleShapeTest, GetSize) {
	Vector2Df position;
	Vector2Df size(50.0f, 50.0f);

	RectangleShape rectangleShape(position, size);

	EXPECT_TRUE(rectangleShape.m_Size == rectangleShape.GetSize());
}

TEST(RectangleShapeTest, SetSize) {
	Vector2Df position;
	Vector2Df size(50.0f, 50.0f);

	RectangleShape rectangleShape(position, size);

	Vector2Df newSize(5.0f, 5.0f);
	rectangleShape.SetSize(newSize);

	EXPECT_TRUE(rectangleShape.m_Size == newSize);
}

TEST(CircleShapeTest, Initialize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;

	CircleShape circleShape(position, numberOfFaces, radius);

	EXPECT_TRUE(circleShape.m_Position == position && circleShape.m_Radius == radius && circleShape.m_NumberOfFaces == numberOfFaces);
}

TEST(CircleShapeTest, Render) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	CircleShape circleShape(position, numberOfFaces, radius);

	Shader &shader = *ResourceManagerInstance.GetShader("ShapesShader");
	EXPECT_NO_THROW(circleShape.Render(shader));
}

TEST(CircleShapeTest, GetNumberOfFaces) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	CircleShape circleShape(position, numberOfFaces, radius);

	EXPECT_TRUE(circleShape.m_NumberOfFaces == circleShape.GetNumberOfFaces());
}

TEST(CircleShapeTest, GenerateFaces) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	CircleShape circleShape(position, numberOfFaces, radius);

	unsigned int newNumberOfFaces = 360;
	circleShape.GenerateFaces(newNumberOfFaces);

	EXPECT_TRUE(circleShape.m_NumberOfFaces == newNumberOfFaces);
}

TEST(CircleShapeTest, GetRadius) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	CircleShape circleShape(position, numberOfFaces, radius);

	EXPECT_TRUE(circleShape.m_Radius == circleShape.GetRadius());
}

TEST(CircleShapeTest, SetRadius) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	CircleShape circleShape(position, numberOfFaces, radius);

	float newRadius = 64.0f;
	circleShape.SetRadius(newRadius);

	EXPECT_TRUE(circleShape.m_Radius == newRadius);
}

TEST(ShapeTest, Initialize) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	EXPECT_TRUE(shapes.size() == 2 && shapes.at(0)->m_Position == position && shapes.at(1)->m_Position == position);
}

TEST(ShapeTest, Render) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	for (auto &shape : shapes) {
		EXPECT_NO_THROW(shape->Render(*ResourceManagerInstance.GetShader("ShapesShader")));
	}
}

TEST(ShapeTest, GetVAOID) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	for (auto &shape : shapes) {
		if (shape->GetVAOID() != shape->m_VAO)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, GetPosition) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	for (auto &shape : shapes) {
		if (shape->GetPosition() != shape->m_Position)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, SetPosition) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	Vector2Df newPostion(500.0f, 200.0f);
	for (auto &shape : shapes) {
		shape->SetPosition(newPostion);
		if (shape->m_Position != newPostion)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, GetRotation) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	for (auto &shape : shapes) {
		if (shape->GetRotation() != shape->m_Rotation)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, SetRotation) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	float newRotation = 45.0f;
	for (auto &shape : shapes) {
		shape->SetRotation(newRotation);
		if (shape->m_Rotation != newRotation)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, GetTexture) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	Texture2D *texture = ResourceManagerInstance.GetTexture("default");
	for (auto &shape : shapes) {
		shape->m_Texture = texture;
		if (shape->m_Texture->m_ID != shape->GetTexture()->m_ID)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, SetTexture) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	Texture2D *newTexture = ResourceManagerInstance.GetTexture("default");
	for (auto &shape : shapes) {
		shape->SetTexture(newTexture);
		if (shape->m_Texture->m_ID != newTexture->m_ID)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, GetColour) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	for (auto &shape : shapes) {
		shape->m_Colour = glm::vec4(0.5f, 0.5f, 0.05, 0.5f);
		if (shape->m_Colour != shape->GetColour())
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, SetColour) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	glm::vec4 newColour = glm::vec4(0.5f, 0.5f, 0.05, 0.5f);
	for (auto &shape : shapes) {
		shape->SetColour(newColour);
		if (shape->m_Colour != newColour)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, GetColourTextureMixValue) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	for (auto &shape : shapes) {
		if (shape->m_ColourTextureMixValue != shape->GetColourTextureMixValue())
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, SetColourTextureMixValue) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	float newColourTextureMixValue = 0.25f;
	for (auto &shape : shapes) {
		shape->SetColourTextureMixValue(newColourTextureMixValue);
		if (shape->m_ColourTextureMixValue != newColourTextureMixValue)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, GetUsingColourAsTextureMask) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	for (auto &shape : shapes) {
		if (shape->m_UsingColourAsTextureMask != shape->GetUsingColourAsTextureMask())
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(ShapeTest, SetUsingColourAsTextureMask) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::vector<std::shared_ptr<Shape>> shapes;

	Vector2Df position;
	unsigned int numberOfFaces = 128;
	float radius = 128.0f;
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<CircleShape>(position, numberOfFaces, radius)));

	Vector2Df size(50.0f, 50.0f);
	EXPECT_NO_THROW(shapes.emplace_back(std::make_shared<RectangleShape>(position, size)));

	bool success = true;
	bool usingColourAsTextureMask = false;
	for (auto &shape : shapes) {
		shape->SetUsingColourAsTextureMask(usingColourAsTextureMask);
		if (shape->m_UsingColourAsTextureMask != usingColourAsTextureMask)
			success = false;
	}
	EXPECT_TRUE(success);
}

TEST(Vector2DTest, Initialize) {
	bool success = true;

	double xValue = 10.5555, yValue = 10.5555;
	Vector2D<double> doubleVector((double)xValue, (double)yValue);
	if (doubleVector.m_XValue != xValue || doubleVector.m_YValue != yValue)
		success = false;

	Vector2D<float> floatVector((float)xValue, (float)yValue);
	if (floatVector.m_XValue != (float)xValue || floatVector.m_YValue != (float)yValue)
		success = false;

	Vector2D<int> intVector((int)xValue, (int)yValue);
	if (intVector.m_XValue != (int)xValue || intVector.m_YValue != (int)yValue)
		success = false;

	Vector2D<long> longVector((long)xValue, (long)yValue);
	if (longVector.m_XValue != (long)xValue || longVector.m_YValue != (long)yValue)
		success = false;

	EXPECT_TRUE(success);
}

TEST(Vector2DTest, InitializeUsingTypedef) {
	bool success = true;

	double xValue = 10.5555, yValue = 10.5555;
	Vector2Dd doubleVector((double)xValue, (double)yValue);
	if (doubleVector.m_XValue != xValue || doubleVector.m_YValue != yValue)
		success = false;

	Vector2Df floatVector((float)xValue, (float)yValue);
	if (floatVector.m_XValue != (float)xValue || floatVector.m_YValue != (float)yValue)
		success = false;

	Vector2Di intVector((int)xValue, (int)yValue);
	if (intVector.m_XValue != (int)xValue || intVector.m_YValue != (int)yValue)
		success = false;

	Vector2Dl longVector((long)xValue, (long)yValue);
	if (longVector.m_XValue != (long)xValue || longVector.m_YValue != (long)yValue)
		success = false;

	EXPECT_TRUE(success);
}

TEST(Vector2DTest, Set) {
	bool success = true;

	double originalXValue = 10.5555, originalYValue = 10.5555;
	double newXValue = 5.444, newYValue = 5.444;
	Vector2Dd doubleVector((double)originalXValue, (double)originalYValue);
	doubleVector.Set(newXValue, newYValue);
	if (doubleVector.m_XValue != newXValue || doubleVector.m_YValue != newYValue)
		success = false;

	Vector2Df floatVector((float)originalXValue, (float)originalYValue);
	floatVector.Set((float)newXValue, (float)newYValue);
	if (floatVector.m_XValue != (float)newXValue || floatVector.m_YValue != (float)newYValue)
		success = false;

	Vector2Di intVector((int)originalXValue, (int)originalYValue);
	intVector.Set((int)newXValue, (int)newYValue);
	if (intVector.m_XValue != (int)newXValue || intVector.m_YValue != (int)newYValue)
		success = false;

	Vector2Dl longVector((long)originalXValue, (long)originalYValue);
	longVector.Set((long)newXValue, (long)newYValue);
	if (longVector.m_XValue != (long)newXValue || longVector.m_YValue != (long)newYValue)
		success = false;

	EXPECT_TRUE(success);
}

TEST(Vector2DTest, Get) {
	bool success = true;

	double xValue = 10.5555, yValue = 10.5555;
	Vector2Dd doubleVector((double)xValue, (double)yValue);
	if (doubleVector.X() != xValue || doubleVector.Y() != yValue)
		success = false;

	Vector2Df floatVector((float)xValue, (float)yValue);
	if (floatVector.X() != (float)xValue || floatVector.Y() != (float)yValue)
		success = false;

	Vector2Di intVector((int)xValue, (int)yValue);
	if (intVector.X() != (int)xValue || intVector.Y() != (int)yValue)
		success = false;

	Vector2Dl longVector((long)xValue, (long)yValue);
	if (longVector.X() != (long)xValue || longVector.Y() != (long)yValue)
		success = false;

	EXPECT_TRUE(success);
}

TEST(Vector2DTest, CalculateMagnitude) {
	float values = 10.0f;
	Vector2Df vector(values, values);

	EXPECT_TRUE(std::sqrt(values * values + values * values) == vector.Magnitude());
}

TEST(Vector2DTest, CalculateDotProduct) {
	float values = 10.0f, otherValues = 2.555f;
	Vector2Df vector(values, values);
	Vector2Df otherVector(otherValues, otherValues);

	EXPECT_TRUE(((vector.m_XValue * otherVector.m_XValue) + (vector.m_YValue * otherVector.m_YValue)) == vector.DotProduct(otherVector));
}

TEST(Vector2DTest, CalculateNormal) {
	float values = 10.0f;
	Vector2Df vector(values, values);

	float magnitude = std::sqrt(values * values + values * values);
	Vector2Df normalVector(values / magnitude, values / magnitude);

	EXPECT_TRUE(vector.Normal().m_XValue == normalVector.m_XValue && vector.Normal().m_YValue == normalVector.m_YValue);
}

TEST(Vector2DTest, CalculatePerpendicular) {
	float values = 10.0f;
	Vector2Df vector(values, values);

	Vector2Df perpendicularVector(values, -values);

	EXPECT_TRUE(vector.Perpendicular().m_XValue == perpendicularVector.m_XValue);
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}