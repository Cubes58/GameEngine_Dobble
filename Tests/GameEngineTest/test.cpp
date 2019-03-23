#include "pch.h"

// Get (direct) access to everything.
#define private public
//#define protected public

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Cursor.hpp>

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

TEST(RenderComponentTest, DefaultInitialization) {
	RenderComponent renderComponent;
	
	EXPECT_NO_THROW(true);
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

TEST(EntityManagerTest, accessInstance) {
	EXPECT_NO_THROW(EntityManager::Instance());
}

TEST(EntityManagerTest, accessInstanceUsingMacro) {
	EXPECT_NO_THROW(EntityManagerInstance);
}

TEST(EntityManagerTest, createComponentWithName) {
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

TEST(EntityManagerTest, createComponentWithEntityID) {
	EntityID entityID = 10;
	EntityManagerInstance.CreateEntity(entityID);

	bool success = false;
	auto iter = EntityManagerInstance.GetEntities()->find(entityID);
	if (iter != EntityManagerInstance.GetEntities()->end())
		success = true;

	EXPECT_TRUE(success);
}

TEST(EntityManagerTest, getEntities) {
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

TEST(EntityManagerTest, addAndGetEntityComponentUsingEntityID) {
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

TEST(EntityManagerTest, addAndGetEntityComponentUsingEntityName) {
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

TEST(EntityManagerTest, deleteComponentUsingEntityID) {
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

TEST(EntityManagerTest, deleteComponentUsingEntityName) {
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

TEST(EntityManagerTest, deleteEntityUsingEntityID) {
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

TEST(EntityManagerTest, deleteEntityUsingEntityName) {
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

TEST(EntityManagerTest, addEntitySystem) {
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

TEST(EntityManagerTest, updateEntitySystem) {
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

TEST(EntityManagerTest, renderEntitySystem) {
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

TEST(EntityManagerTest, clearEntityManagerData) {
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

TEST(AudioManagerTest, accessInstance) {
	EXPECT_NO_THROW(AudioManager::Instance());
}

TEST(AudioManagerTest, accessInstanceUsingMacro) {
	EXPECT_NO_THROW(AudioManagerInstance);
}

TEST(AudioManagerTest, openMusicFile) {
	bool success = false;

	EXPECT_NO_THROW(success = AudioManagerInstance.OpenMusicFile("resources/audio/music/dobbleTheme.wav", true));
	if (!AudioManagerInstance.m_BackgroundMusic.m_isStreaming)
		success = false;

	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, loadSoundEffectFileWithNameAndFile) {
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

TEST(AudioManagerTest, loadSoundEffectFileWithJustFile) {
	bool success = false;

	std::string soundEffectName = "symbolGuess";
	EXPECT_NO_THROW(success = AudioManagerInstance.LoadSoundEffect("resources/audio/soundEffects/" + soundEffectName + ".wav"));

	auto iter = AudioManagerInstance.m_SoundEffects.find(soundEffectName);
	if (iter == AudioManagerInstance.m_SoundEffects.end()) {
		success = false;
	}

	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, loadSoundEffectsFromFolder) {
	bool success = false;
	// Load files, even if there aren't any to load there shouldn't be an exception thrown!
	EXPECT_NO_THROW(success = AudioManagerInstance.LoadSoundEffects("resources/audio/soundEffects/"));

	// Ensure the files were loaded.
	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, playSoundEffect) {
	bool success = false;

	std::string soundEffectName = "bleep";
	EXPECT_NO_THROW(AudioManagerInstance.LoadSoundEffect("bleep", "resources/audio/soundEffects/" + soundEffectName + ".wav"));
	EXPECT_NO_THROW(success = AudioManagerInstance.PlaySoundEffect(soundEffectName));

	// Ensure the files were loaded.
	EXPECT_TRUE(success);
}

TEST(AudioManagerTest, getSoundEffect) {
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

TEST(AudioManagerTest, getBackgroundMusic) {
	bool success = false;

	EXPECT_NO_THROW(AudioManagerInstance.OpenMusicFile("resources/audio/music/dobbleTheme.wav", true));

	AudioManagerInstance.GetBackgroundMusic().stop();
	if (AudioManagerInstance.m_BackgroundMusic.getStatus() == sf::Music::Stopped) {
		success = true;
	}

	// Ensure the files were loaded.
	EXPECT_TRUE(success);
}

TEST(ParticleManagerTest, initializeData) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	std::shared_ptr<ParticleManager> particleManager;
	EXPECT_NO_THROW(particleManager = std::make_shared<ParticleManager>(screenSize, 50));

	bool success = true;
	if (particleManager->m_NumberOfParticles != 50)
		success = false;

	EXPECT_TRUE(success);
}

TEST(ParticleManagerTest, updateParticles) {
	Vector2Df screenSize(1280.0f, 720.0f);
	Window window(sf::VideoMode((int)screenSize.X(), (int)screenSize.Y()), "Dobble", sf::Style::Close);

	ParticleManager particleManager(screenSize, 50);

	bool success = true;
	particleManager.Update(60.0f / 1.0f, Vector2Df(0.0f, 0.0f));
	
	//if (particleManager. != firstParticleUsedSet) {
		success = false;
	//}

	EXPECT_TRUE(success);
}

TEST(MouseInputTest, mousePosition) {
	sf::Mouse mouseEvent;
	sf::Vector2i givenPosition = sf::Vector2i(10, 10);
	mouseEvent.setPosition(givenPosition);

	sf::Vector2i mousePosition;
	mousePosition = mouseEvent.getPosition();

	EXPECT_TRUE(givenPosition == mousePosition);
}

TEST(MouseInputTest, symbolGuess) {
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

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}