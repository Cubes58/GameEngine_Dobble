#include "Deck.h"

#include <algorithm>
#include <random>
#include <climits>

#include "EntityManager.h"
#include "Randomiser.h"

#include "CollisionSystem.h"
#include "MoveSymbolsSystem.h"

#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "RenderComponent.h"

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

void Deck::GenerateSymbolData(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard) {
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
			// Generate the position.
			circleTransformData.m_Position = Vector2D<float>(Randomiser::Instance().GetUniformRealRandomNumber(-p_CardRadius, p_CardRadius), 
				Randomiser::Instance().GetUniformRealRandomNumber(-p_CardRadius, p_CardRadius));

			// Generate the orientation.
			circleTransformData.m_Rotation = Randomiser::Instance().GetNormalRandomNumber(0.0f, 360.0f);

			// Generate the size.
			circleTransformData.m_Radius = Randomiser::Instance().GetNormalRandomNumber(22.5f, 40.0f);

			// Add the symbol's transform component.
			circleTransforms.emplace_back(circleTransformData);
		}
		EntityManagerInstance.AddComponentToEntity(entity, std::make_shared<TransformComponent>(circleTransforms));
	}
}

void Deck::GenerateCards(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard) {
	EntityManagerInstance.Clear();

	GenerateCardSymbolIDs(p_NumberOfSymblesPerCard);
	GenerateSymbolData(p_CardPosition, p_CardRadius, p_NumberOfSymblesPerCard);

	std::shared_ptr<CollisionSystem> collisionSystem = std::make_shared<CollisionSystem>();
	bool &SymbolsSuccessfullyWithinCards = collisionSystem->SymbolsSuccessfullyWithinCards();
	EntityManagerInstance.AddSystem(collisionSystem);

	std::shared_ptr<MoveSymbolsSystem> moveSymbolsSystem = std::make_shared<MoveSymbolsSystem>();
	EntityManagerInstance.AddSystem(moveSymbolsSystem);

	/* Need to finish the MoveSymbolsSystem
	while (!SymbolsSuccessfullyWithinCards) {
		EntityManagerInstance.UpdateSystems(1.0f);
	}*/

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