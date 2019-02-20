#pragma once

#include <vector>
#include <climits>

#include "Vector2D.h"

class Deck {
private:
	// Maybe have a vector, which stores key ids - shuffles that, then use that as the "deck", so get ids from that and send them to the clients.
	std::vector<unsigned int> m_EntityOrder;
	
	bool GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard);
	void GenerateSymbolData(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);

public:
	Deck() = default;
	~Deck() = default;

	void GenerateCards(Vector2D<float> p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);
	void Shuffle();

	bool IsDeckEmpty() const;
	unsigned int GetEntityID();
	std::vector<unsigned int> &GetEntityList();
};