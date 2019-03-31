#include "pch.h"

#define private public

#include "ServerGame.h"
#include "Randomiser.h"

#include "EntityManager.h"
#include "Deck.h"

TEST(RandomiserTest, UniformRealRandomNumberGenerated) {
	float min = 0.5f;
	float max = 10.0f;

	float realRandomNumber = RandomiserInstance.GetUniformRealRandomNumber(min, max);

	EXPECT_TRUE((realRandomNumber >= min) && (realRandomNumber <= max));
}

TEST(RandomiserTest, UniformIntegerRandomNumberGenerated) {
	int min = 5;
	int max = 10;

	int integerRandomNumber = RandomiserInstance.GetUniformIntegerRandomNumber(min, max);

	EXPECT_TRUE((integerRandomNumber >= min) && (integerRandomNumber <= max));
}

TEST(RandomiserTest, NormalRandomNumberGenerated) {
	float min = 0.5f;
	float max = 10.0f;

	float integerRandomNumber = RandomiserInstance.GetNormalRandomNumber(min, max);

	EXPECT_TRUE((integerRandomNumber >= min) && (integerRandomNumber <= max));
}

TEST(CollisionTest, CircleCircle) {

}

TEST(CollisionTest, CirclePoint) {

}

TEST(CollisionTest, AABBAABB) {

}

TEST(CollisionTest, AABBPoint) {

}

TEST(CollisionTest, IsCircleInCircle) {

}

TEST(ServerGameTest, Initialize) {
	std::shared_ptr<ServerGame> serverGame;
	EXPECT_NO_THROW(serverGame = std::make_shared<ServerGame>());

}

TEST(ServerGameTest, Update) {

}

TEST(ServerGameTest, IsRunning) {

}

TEST(ServerTest, Initialize) {

}

TEST(ServerTest, CheckForClientConnectionRequest) {

}

TEST(ServerTest, WaitForClientsToConnect) {

}

TEST(ServerTest, Disconnect) {

}

TEST(ServerTest, Send) {

}

TEST(ServerTest, GetReceivedData) {

}

TEST(ServerTest, GetListeningState) {

}

TEST(ServerTest, SetListeningState) {

}

TEST(ServerTest, GetClientIDs) {

}

TEST(ServerTest, GetNumberOfClients) {

}

TEST(ServerTest, IsRunning) {

}

TEST(DeckTest, GenerateCards) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	bool singleSymbolMatchPerCardPair = true;
	for (unsigned int i = 0; i < deck.m_CardIDs.size(); i++) {
		for (unsigned int j = 0; j < deck.m_CardIDs.size(); j++) {
			if (i == j)
				continue;

			RenderComponent renderComponent = *EntityManagerInstance.GetComponent<RenderComponent>(deck.m_CardIDs[i]);
			RenderComponent otherRenderComponent = *EntityManagerInstance.GetComponent<RenderComponent>(deck.m_CardIDs[j]);

			int numberOfMatchingSymbols = 0;
			for (unsigned int k = 0; k < renderComponent.m_SymbolTextureIDs.size(); k++) {
				for (unsigned int l = 0; l < otherRenderComponent.m_SymbolTextureIDs.size(); l++) {
					if (renderComponent.m_SymbolTextureIDs[k] == otherRenderComponent.m_SymbolTextureIDs[l]) {
						numberOfMatchingSymbols++;
					}
				}
			}

			// Two because both cards will have that symbol - every symbol is looped over twice.
			// Anymore than two then there's more than a single matching symbol.
			// Any less than two then there's no matching symbol.
			if (numberOfMatchingSymbols > 2 || numberOfMatchingSymbols < 2) {
				singleSymbolMatchPerCardPair = false;
			}
		}
	}

	EXPECT_TRUE(singleSymbolMatchPerCardPair);
}

TEST(DeckTest, Shuffle) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	std::vector<unsigned int> startingOrderOfCardIDs = deck.m_CardIDs;
	deck.Shuffle();
	std::vector<unsigned int> shuffledOrderOfCardIDs = deck.m_CardIDs;

	bool success = true;
	unsigned int numberOfCardIDsInSamePosition = 0;
	const unsigned int maxNumberOfCardIDsInSamePositionToConsiderFailure = static_cast<unsigned int>(deck.m_CardIDs.size() / 10);
	for (unsigned int i = 0; i < deck.m_CardIDs.size(); ++i) {
		if (startingOrderOfCardIDs[i] == shuffledOrderOfCardIDs[i]) 
			++numberOfCardIDsInSamePosition;
	}

	if (numberOfCardIDsInSamePosition >= maxNumberOfCardIDsInSamePositionToConsiderFailure)
		success = false;


	EXPECT_TRUE(success);
}

TEST(DeckTest, HasMatchingSymbol) {
	std::shared_ptr<RenderComponent> renderComponent = std::make_shared<RenderComponent>();

	unsigned int numberOfSymbolsPerCard = 8;
	for (int i = 0; i < numberOfSymbolsPerCard; ++i) {
		renderComponent->m_SymbolTextureIDs.emplace_back(i);
	}
	unsigned int symbolIDToMatch = --numberOfSymbolsPerCard;

	EXPECT_TRUE(Deck::HasMatchingSymbol(renderComponent, symbolIDToMatch));
}

TEST(DeckTest, IsDeckEmpty) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	bool success = true;

	if (deck.m_CardIDs.size() > 0 && deck.IsDeckEmpty())
		success = false;

	// Clear the deck, so empty should return true.
	deck.m_CardIDs.clear();

	if (!deck.IsDeckEmpty())
		success = false;

	EXPECT_TRUE(success);
}

TEST(DeckTest, NumberOfRemainingCards) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	EXPECT_TRUE(deck.NumberOfRemainingCards() == deck.m_CardIDs.size());
}

TEST(DeckTest, GetCardIDFromTop) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	// Get the last card from the deck.
	unsigned int topCardID = *(--deck.m_CardIDs.end());

	EXPECT_TRUE(topCardID == deck.GetCardIDFromTop());
}

TEST(DeckTest, GetCardIDs) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2Df(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

	EXPECT_TRUE(deck.m_CardIDs.size() == deck.GetCardIDs().size());
}

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}