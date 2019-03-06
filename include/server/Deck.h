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
	
	//std::vector<Vector2D<float>> GeneratePositionsWithinCircle(Vector2D<float> p_CirclePosition, float p_CircleRadius);
	bool GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard);
	void GenerateSymbolTransformData(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);

	//bool IsPositionWithinCircle(Vector2D<float> p_CirclePosition, float p_CircleRadius, Vector2D<float> p_Position);
	//bool IsCircleWithinCircle(Vector2D<float> p_CirclePosition, float p_CircleRadius, Vector2D<float> p_Position, float p_Radius);
	std::vector<Vector2D<float>> CreateDirectionLine(Vector2D<float> p_CirclePosition, float p_LineLength, float p_Angle);

public:
	Deck() = default;
	~Deck() = default;

	void GenerateCards(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);
	void Shuffle();

	static bool HasMatchingSymbol(std::shared_ptr<RenderComponent> p_DeckCardRenderComponent, unsigned int p_PlayerSymbolIDGuess);

	bool IsDeckEmpty() const;
	unsigned int NumberOfRemainingCards() const;
	unsigned int GetCardIDFromTop();
	std::vector<unsigned int> &GetCardIDs();
};