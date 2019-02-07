#include "Deck.h"

bool Deck::GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard) {
	static constexpr unsigned int s_ConstantIncrement = 1;
	// Check if the given number is prime;
	p_NumberOfSymblesPerCard -= s_ConstantIncrement;
	for (int i = 2; i <= p_NumberOfSymblesPerCard / 2; i++) {
		if (p_NumberOfSymblesPerCard % i == 0) {
			return false;
		}
	}
	// Reserve enough space for the number of cards that'll be generated.
	m_Cards.reserve((p_NumberOfSymblesPerCard * p_NumberOfSymblesPerCard) - (p_NumberOfSymblesPerCard - 1));

	unsigned int cardNumber = 0;
	// First card.
	m_Cards.emplace_back(++cardNumber);
	for (int i = 0; i <= p_NumberOfSymblesPerCard; i++)
		m_Cards[cardNumber - 1].m_SymbolIDs.emplace_back(i + s_ConstantIncrement);
	
	// N following cards.
	for (int i = 0; i < p_NumberOfSymblesPerCard; i++) {
		m_Cards.emplace_back(++cardNumber);

		//m_Cards[cardNumber - 1].m_SymbolIDs[0] = s_ConstantIncrement;
		m_Cards[cardNumber - 1].m_SymbolIDs.emplace_back(s_ConstantIncrement);
		for (int j = 0; j < p_NumberOfSymblesPerCard; j++)
			m_Cards[cardNumber - 1].m_SymbolIDs.emplace_back((p_NumberOfSymblesPerCard + p_NumberOfSymblesPerCard * i + j) + (s_ConstantIncrement + s_ConstantIncrement));
			//m_Cards[cardNumber - 1].m_SymbolIDs[j + 1] = (p_NumberOfSymblesPerCard + p_NumberOfSymblesPerCard * i + j) + (s_ConstantIncrement + s_ConstantIncrement);
	}

	// N*N following cards
	for (int i = 0; i < p_NumberOfSymblesPerCard; i++) {
		for (int j = 0; j < p_NumberOfSymblesPerCard; j++) {
			m_Cards.emplace_back(++cardNumber);

			m_Cards[cardNumber - 1].m_SymbolIDs.emplace_back(i + s_ConstantIncrement + s_ConstantIncrement);
			for (int k = 0; k < p_NumberOfSymblesPerCard; k++) 
				m_Cards[cardNumber - 1].m_SymbolIDs.emplace_back(((p_NumberOfSymblesPerCard + p_NumberOfSymblesPerCard * k + (i * k + j) % p_NumberOfSymblesPerCard)
					+ s_ConstantIncrement + s_ConstantIncrement));	// Works properly as long as p_NumberOfSymblesPerCard is a prime number.
		}
	}

	return true;
}

const std::vector<Card>& Deck::GenerateCards(unsigned int p_NumberOfSymblesPerCard, float p_CardRadius) {
	GenerateCardSymbolIDs(p_NumberOfSymblesPerCard);

	return m_Cards;
}
