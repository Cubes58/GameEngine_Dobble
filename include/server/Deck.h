#pragma once

#include <vector>
#include <memory>

#include "Vector2D.h"
#include "Collision.h"

struct RenderComponent;

class Deck {
private:
	std::vector<unsigned int> m_CardIDs;
	Collision m_Collision;
	
	bool GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard);
	void GenerateSymbolTransformData(Vector2Df p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);
	std::vector<Vector2Df> CreateDirectionLine(Vector2Df p_CirclePosition, float p_LineLength, float p_Angle);

public:
	Deck() = default;
	~Deck() = default;

	void GenerateCards(Vector2Df p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);
	void Shuffle();

	static bool HasMatchingSymbol(std::shared_ptr<RenderComponent> p_DeckCardRenderComponent, unsigned int p_PlayerSymbolIDGuess);

	bool IsDeckEmpty() const;
	unsigned int NumberOfRemainingCards() const;
	unsigned int GetCardIDFromTop();
	std::vector<unsigned int> &GetCardIDs();
};