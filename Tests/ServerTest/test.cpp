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

TEST(SymbolGeneration, SingleSymbolMatchPerCardPair) {
	Deck deck;
	unsigned int numberOfSymbolsPerCard = 8;
	deck.GenerateCards(Vector2D<float>(0.0f, 0.0f), 250.0f, numberOfSymbolsPerCard);

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

int main(int ac, char *av[]) {
	testing::InitGoogleTest(&ac, av);
	return RUN_ALL_TESTS();
}