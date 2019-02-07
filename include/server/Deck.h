#pragma once

#include <vector>

struct Card {
	unsigned int m_CardNumber;
	std::vector<unsigned int> m_SymbolIDs;

	Card() = default;
	Card(const unsigned int &p_CardNumber) : m_CardNumber(p_CardNumber) {}
};

class Deck {
private:
	std::vector<Card> m_Cards;

	bool GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard);

public:
	Deck() = default;
	~Deck() = default;

	const std::vector<Card> &GenerateCards(unsigned int p_NumberOfSymblesPerCard, float p_CardRadius);
};