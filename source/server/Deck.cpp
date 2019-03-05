#include "Deck.h"

#include <algorithm>
#include <random>
#include <climits>

#define _USE_MATH_DEFINES
#include <math.h>

#include "EntityManager.h"
#include "Randomiser.h"

#include "CollisionSystem.h"
#include "MoveSymbolsSystem.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "RenderComponent.h"

std::vector<Vector2D<float>> Deck::GeneratePositionsWithinCircle(Vector2D<float> p_CirclePosition, float p_CircleRadius) {
	std::vector<Vector2D<float>> m_CirclePositions;
	unsigned int numberOfPositions = static_cast<unsigned int>(M_PI * (p_CircleRadius * p_CircleRadius));
	m_CirclePositions.reserve(numberOfPositions);

	float circleLeft = p_CirclePosition.X() - p_CircleRadius;		// Left of the screen is 0
	float circleRight = p_CirclePosition.X() + p_CircleRadius;

	float circleTop = p_CirclePosition.Y() - p_CircleRadius;		// Top of screen is 0
	float circleBottom = p_CirclePosition.Y() + p_CircleRadius;

	for (float yPosition = circleTop; yPosition <= circleBottom; yPosition++) {
		for (float xPosition = circleLeft; xPosition <= circleRight; xPosition++) {
			Vector2D<float> position(xPosition, yPosition);

			if (IsPositionWithinCircle(p_CirclePosition, p_CircleRadius, position)) {
				m_CirclePositions.emplace_back(position);
			}
		}
	}

	return m_CirclePositions;
}

bool Deck::GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard) {
	static constexpr unsigned int s_ConstantIncrement = 1;
	// Check if the given number is prime;
	p_NumberOfSymblesPerCard -= s_ConstantIncrement;
	for (int i = 2; i <= p_NumberOfSymblesPerCard / 2; i++) {
		if (p_NumberOfSymblesPerCard % i == 0) {
			Log(Type::FAULT) << "Not a prime number: " << p_NumberOfSymblesPerCard;
			return false;
		}
	}

	unsigned int cardNumber = 0;
	RenderComponent renderComponent;
	renderComponent.m_SymbolTextureIDs.reserve(p_NumberOfSymblesPerCard);

	// First card.
	EntityManagerInstance.CreateEntity(++cardNumber);
	renderComponent.m_SymbolTextureIDs.emplace_back(0);	// Background render ID.
	for (int i = 0; i <= p_NumberOfSymblesPerCard; i++)
		renderComponent.m_SymbolTextureIDs.emplace_back(i + s_ConstantIncrement);
	EntityManagerInstance.AddComponentToEntity<RenderComponent>(cardNumber, std::make_shared<RenderComponent>(renderComponent));

	// N following cards.
	for (int i = 0; i < p_NumberOfSymblesPerCard; i++) {
		renderComponent.m_SymbolTextureIDs.clear();	// Removes the elements but the capacity (reserved memory) remains.

		EntityManagerInstance.CreateEntity(++cardNumber);
		renderComponent.m_SymbolTextureIDs.emplace_back(0);	// Background render ID.
		renderComponent.m_SymbolTextureIDs.emplace_back(s_ConstantIncrement);

		for (int j = 0; j < p_NumberOfSymblesPerCard; j++)
			renderComponent.m_SymbolTextureIDs.emplace_back((p_NumberOfSymblesPerCard + p_NumberOfSymblesPerCard * i + j) + (s_ConstantIncrement + s_ConstantIncrement));

		EntityManagerInstance.AddComponentToEntity<RenderComponent>(cardNumber, std::make_shared<RenderComponent>(renderComponent));
	}

	// N*N following cards
	for (int i = 0; i < p_NumberOfSymblesPerCard; i++) {
		for (int j = 0; j < p_NumberOfSymblesPerCard; j++) {
			renderComponent.m_SymbolTextureIDs.clear();	
			EntityManagerInstance.CreateEntity(++cardNumber);
			renderComponent.m_SymbolTextureIDs.emplace_back(0);	// Background render ID.

			renderComponent.m_SymbolTextureIDs.emplace_back(i + s_ConstantIncrement + s_ConstantIncrement);
			for (int k = 0; k < p_NumberOfSymblesPerCard; k++)
				renderComponent.m_SymbolTextureIDs.emplace_back(((p_NumberOfSymblesPerCard + p_NumberOfSymblesPerCard * k + (i * k + j) % p_NumberOfSymblesPerCard)
					+ s_ConstantIncrement + s_ConstantIncrement));	// Works properly as long as (p_NumberOfSymblesPerCard - 1) is a prime number.

			EntityManagerInstance.AddComponentToEntity<RenderComponent>(cardNumber, std::make_shared<RenderComponent>(renderComponent));
		}
	}

	return true;
}

void Deck::GenerateSymbolTransformData(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard) {
	float minimumCircleRadius = (p_CardRadius / p_NumberOfSymblesPerCard) / 1.2f;
	float maximumCircleRadius = (p_CardRadius / p_NumberOfSymblesPerCard) / 0.955f;

	//m_ValidSymbolStartingPositions = GeneratePositionsWithinCircle(p_CardPosition, p_CardRadius * 0.25f);

	p_NumberOfSymblesPerCard += 1;	// Add one for the card background (position/collision test).
	const std::set<EntityID> *entities = EntityManagerInstance.GetEntities();
	for (const auto &entity : *entities) {
		// Generate collision and transform components.
		EntityManagerInstance.AddComponentToEntity(entity, std::make_shared<CollisionComponent>(p_NumberOfSymblesPerCard));
		
		std::vector<CircleTransformData> circleTransforms;
		circleTransforms.reserve(p_NumberOfSymblesPerCard);
		// Add the card background (First element in the render component is always the background).
		circleTransforms.emplace_back(p_CardPosition, p_CardRadius);

		for (int i = 0; i < p_NumberOfSymblesPerCard - 1; i++) {
			CircleTransformData circleTransformData;
			
			circleTransformData.m_Position = Vector2D<float>(RandomiserInstance.GetUniformRealRandomNumber(-p_CardRadius, p_CardRadius),
				RandomiserInstance.GetUniformRealRandomNumber(-p_CardRadius, p_CardRadius));
			
			// Generate the orientation.
			circleTransformData.m_Rotation = RandomiserInstance.GetUniformRealRandomNumber(0.0f, 360.0f);

			// Generate the size.
			circleTransformData.m_Radius = RandomiserInstance.GetUniformRealRandomNumber(minimumCircleRadius, maximumCircleRadius);

			// Add the symbol's transform component.
			circleTransforms.emplace_back(circleTransformData);
		}
		EntityManagerInstance.AddComponentToEntity(entity, std::make_shared<TransformComponent>(circleTransforms));
	}
}

void Deck::GenerateCards(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard) {
	EntityManagerInstance.Clear();

	GenerateCardSymbolIDs(p_NumberOfSymblesPerCard);
	GenerateSymbolTransformData(p_CardPosition, p_CardRadius, p_NumberOfSymblesPerCard);

	// Vector to store a shuffled deck of entity IDs, so when it comes to sending out a card, it will be a "random" entity, chosen from the back of this vector.
	const std::set<EntityID> *entities = EntityManagerInstance.GetEntities();
	m_CardIDs.reserve(entities->size());
	for (const auto &entity : *entities) {
		m_CardIDs.emplace_back(entity);
	}

	Shuffle();
}

void Deck::Shuffle() {
	std::shuffle(m_CardIDs.begin(), m_CardIDs.end(), Randomiser::Instance().Generator());
}

bool Deck::HasMatchingSymbol(std::shared_ptr<RenderComponent> p_DeckCardRenderComponent, unsigned int p_PlayerSymbolIDGuess) {
	for (const auto &deckCardSymbolID : p_DeckCardRenderComponent->m_SymbolTextureIDs) {
		if (deckCardSymbolID == p_PlayerSymbolIDGuess) {
			Log(Type::INFO) << "The player guessed correctly!" << "\t" << "Player guess: (Symbol ID) " << p_PlayerSymbolIDGuess;
			return true;
		}
	}

	return false;
}

bool Deck::IsPositionWithinCircle(Vector2D<float> p_CirclePosition, float p_CircleRadius, Vector2D<float> p_Position) {
	Vector2D<float> distance(p_CirclePosition - p_Position);
	float squareDist = distance.DotProduct(distance);

	return squareDist < p_CircleRadius * p_CircleRadius;
}

bool Deck::IsCircleWithinCircle(Vector2D<float> p_CirclePosition, float p_CircleRadius, Vector2D<float> p_Position, float p_Radius) {
	Vector2D<float> distance(p_CirclePosition - p_Position);
	float squareDist = distance.DotProduct(distance);

	return squareDist < ((p_CircleRadius * p_CircleRadius) + (p_Radius * p_Radius));
}

std::vector<Vector2D<float>> Deck::CreateDirectionLine(Vector2D<float> p_CirclePosition, float p_LineLength, float p_Angle) {
	std::vector<Vector2D<float>> line;
	line.reserve((unsigned int)((unsigned int)p_LineLength * (unsigned int)p_LineLength));

	for (float length = 0.0f; length < p_LineLength; length++)
		line.emplace_back(length * cos(p_Angle), length * sin(p_Angle));

	return line;
}

bool Deck::IsDeckEmpty() const {
	if (m_CardIDs.size() <= 0)
		return true;

	return false;
}

unsigned int Deck::NumberOfRemainingCards() const {
	return m_CardIDs.size();
}

unsigned int Deck::GetCardIDFromTop() {
	if (m_CardIDs.size() <= 0)
		return UINT_MAX;

	unsigned int backID = *(m_CardIDs.end() - 1);
	m_CardIDs.pop_back();
	return backID;
}

std::vector<unsigned int> &Deck::GetCardIDs() {
	return m_CardIDs;
}