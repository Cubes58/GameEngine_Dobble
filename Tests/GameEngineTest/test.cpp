#include "pch.h"

#define private public

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "Collision.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Cursor.hpp>

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
	TransformComponent transformComponent;

	EXPECT_NO_THROW(true);
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
		circleTransformData.emplace_back(Vector2D<float>((float)i, (float)i), i * 10.0f, i * 15.0f);
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
		circleTransformData.emplace_back(Vector2D<float>((float)i, (float)i), i * 10.0f, i * 15.0f);
	}
	TransformComponent transformComponent(circleTransformData);

	bool sameData = true;
	if (transformComponent.m_CircleTransforms.size() != circleTransformData.size()) {
		sameData = false;
	}

	circleTransformData[0].m_Position = Vector2D<float>(1.0f, 1000.0f);

	for (unsigned int i = 0; i < numberOfCirclesPerCard; i++) {
		if (circleTransformData[i] != transformComponent.m_CircleTransforms[i]) {
			sameData = false;
		}
	}

	EXPECT_TRUE(!sameData);
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
		circleTransformData.emplace_back(Vector2D<float>((float)i, (float)i), i * 10.0f, i * 15.0f);
	}
	TransformComponent transformComponent(circleTransformData);
	transformComponent.m_CircleTransforms[0].m_Position = Vector2D<float>(10.0f, 10.0f);
	transformComponent.m_CircleTransforms[0].m_Radius = 50.0f;

	bool success = false;
	Collision collisionChecker;
	if (collisionChecker(transformComponent.m_CircleTransforms[0].m_Position, transformComponent.m_CircleTransforms[0].m_Radius, Vector2D<float>(mousePosition.x, mousePosition.y)))
		success = true;

	EXPECT_TRUE(success);
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}