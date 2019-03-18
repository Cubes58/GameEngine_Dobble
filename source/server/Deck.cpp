#include "Deck.h"

#include <algorithm>
#include <random>
#include <climits>

#define _USE_MATH_DEFINES
#include <math.h>

#include "EntityManager.h"
#include "Randomiser.h"

#include "TransformComponent.h"
#include "RenderComponent.h"

#include "Logger.h"

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

void Deck::GenerateSymbolTransformData(Vector2Df p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard) {
	float minimumCircleRadius = (p_CardRadius / p_NumberOfSymblesPerCard) / 0.95f;
	float maximumCircleRadius = (p_CardRadius / p_NumberOfSymblesPerCard) / 0.65f;

	p_NumberOfSymblesPerCard += 1;	// Add one for the card background (position/collision test).
	const std::set<EntityID> *entities = EntityManagerInstance.GetEntities();
	for (const auto &entity : *entities) {
		// Generate transform components.		
		std::vector<CircleTransformData> circleTransforms;
		circleTransforms.reserve(p_NumberOfSymblesPerCard);
		// Add the card background (First element in the render component is always the background).
		circleTransforms.emplace_back(p_CardPosition, p_CardRadius);

		for (int i = 0; i < p_NumberOfSymblesPerCard - 1; i++) {
			CircleTransformData circleTransformData;
			
			// Generate the orientation.
			circleTransformData.m_Rotation = RandomiserInstance.GetUniformRealRandomNumber(0.0f, 360.0f);

			// Generate the size.
			circleTransformData.m_Radius = RandomiserInstance.GetUniformRealRandomNumber(maximumCircleRadius * 1.5f, maximumCircleRadius * 2.0f);

			// Generate the position.
			bool inOtherCircle = false;
			unsigned int numberOfAttemps = 0;
			unsigned int totalNumberOfAttempts = 0;
			unsigned int indexOfCircleToShrink = 2;
			do {
				circleTransformData.m_Position = Vector2Df(RandomiserInstance.GetUniformRealRandomNumber(-p_CardRadius + circleTransformData.m_Radius, p_CardRadius - circleTransformData.m_Radius),
					RandomiserInstance.GetUniformRealRandomNumber(-p_CardRadius + circleTransformData.m_Radius, p_CardRadius - circleTransformData.m_Radius));

				for (int i = 1; i < circleTransforms.size(); i++) {
					if (m_Collision(circleTransformData.m_Position, circleTransformData.m_Radius, circleTransforms[i].m_Position, circleTransforms[i].m_Radius)) {
						inOtherCircle = true;
						break;
					}
					else
						inOtherCircle = false;
				}
				++numberOfAttemps;
				++totalNumberOfAttempts;

				if (numberOfAttemps >= MAX_NUMBER_OF_ATTEMPTS_BEFORE_SHRINKING_CIRCLE) {
					// If the circle is smaller than half of the minimum, then shrink one of the other circles on the card.
					if (circleTransformData.m_Radius < minimumCircleRadius / 2.0f) {
						if (indexOfCircleToShrink == circleTransforms.size()) {
							indexOfCircleToShrink = 2;
						}

						circleTransforms[indexOfCircleToShrink].m_Radius = circleTransforms[indexOfCircleToShrink].m_Radius / 2.0f;	// Half the radius of a circle, on the card.
						++indexOfCircleToShrink;
					}
					else
						circleTransformData.m_Radius = circleTransformData.m_Radius / 2.0f;	// Shrink the circle.

					numberOfAttemps = 0;	// Attempt to add the card again.
				}

			} while (!m_Collision.IsCircleInCircle(p_CardPosition, p_CardRadius, circleTransformData.m_Position, circleTransformData.m_Radius) || inOtherCircle);

			Log(Type::INFO) << "Number of attempts to place the symbol ID: " << i << " within the entity: " << entity << " is equal to: " << numberOfAttemps;
			Log(Type::INFO) << "Total number of attempts to place the symbol = " << totalNumberOfAttempts;


			// Add the symbol's transform component.
			circleTransforms.emplace_back(circleTransformData);
		}
		ImproveTransformData(circleTransforms, minimumCircleRadius, maximumCircleRadius);

		EntityManagerInstance.AddComponentToEntity(entity, std::make_shared<TransformComponent>(circleTransforms));
	}

	Log(Type::INFO) << "All of the card entities have been created.";
}

void Deck::GenerateCards(Vector2Df p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard) {
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
	std::shuffle(m_CardIDs.begin(), m_CardIDs.end(), RandomiserInstance.Generator());
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

std::vector<Vector2Df> Deck::CreateDirectionLine(Vector2Df p_CirclePosition, float p_LineLength, float p_Angle) {
	std::vector<Vector2Df> line;
	line.reserve((unsigned int)(p_LineLength * p_LineLength));

	for (float length = 0.0f; length < p_LineLength; length++)
		line.emplace_back(length * cos(p_Angle), length * sin(p_Angle));

	return line;
}

bool Deck::ImproveTransformData(std::vector<CircleTransformData> &p_CircleTransforms, float p_MinimumRadius, float p_MaximumRadius) {
	// Find the smallest circle
	// Check to see how far away all of the others circles are, and how close it is to the edge of the card.
	// Grow the circle, as much as possible. Stop if/when it hits the maximum radius, or if it collides with another circle, or leaves the card's area.

	// KEEP TRACK OF THE CIRCLES THAT'VE BEEN CHANGED - don't try to improve them again here.
	// If further improvement it wanted then call the method again, on the same component, with the same or different min/max values.

	// Move to the next cirle that's smaller than the rest.
	// Improve the size of this.

	return false;
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