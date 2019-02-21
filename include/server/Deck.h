#pragma once

#include <vector>
#include <memory>

#include "Vector2D.h"

struct RenderComponent;

class Deck {
private:
	std::vector<unsigned int> m_CardOrder;
	
	bool GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard);
	void GenerateSymbolData(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);

public:
	Deck() = default;
	~Deck() = default;

	void GenerateCards(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);
	void Shuffle();
	static bool HasMatchingSymbol(std::shared_ptr<RenderComponent> p_DeckCardRenderComponent, unsigned int p_PlayerSymbolIDGuess);

	bool IsDeckEmpty() const;
	unsigned int GetCardIDFromTop();
	std::vector<unsigned int> &GetCardIDs();
};