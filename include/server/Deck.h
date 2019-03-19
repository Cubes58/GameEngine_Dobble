/**
@file Deck.h
@brief Manages the card entities.
*/
#pragma once

#include <vector>
#include <memory>

#include "Vector2D.h"
#include "Collision.h"

struct RenderComponent;
struct CircleTransformData;

#define MAX_NUMBER_OF_ATTEMPTS_BEFORE_SHRINKING_CIRCLE 250	//!< The maximum number of attempts, to place the circle within the card, before shrinking it.

/*! \class Deck
	\brief Manages the card entities.
*/

class Deck {
private:
	std::vector<unsigned int> m_CardIDs;	//!< A vector of the card entity IDs, for managing.
	Collision m_Collision;	//!< Checks for different types of collisions.
	
	/*!
		\brief Generates the render component for the card entities.
		\param p_NumberOfSymblesPerCard The number of symbols the card will contain.
		\return Returns true if the symbols were all successfully generated, false otherwise.
	*/
	bool GenerateCardSymbolIDs(unsigned int p_NumberOfSymblesPerCard);
	/*!
		\brief Generates the transform data for the cards' symbols.
		\param p_CardPosition The position of the card.
		\param p_CardRadius The radius of the card.
		\param p_NumberOfSymblesPerCard The number of symbols each card has.
		\return Nothing.
	*/
	void GenerateSymbolTransformData(Vector2Df p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);
	/*!
		\brief Creates a direction line.
		\param p_CirclePosition The positiong of the circle.
		\param p_LineLength The length of the line.
		\param p_Angle The angle to create the line from.
		\return Returns a vector of points, which create a direction line.
	*/
	std::vector<Vector2Df> CreateDirectionLine(Vector2Df p_CirclePosition, float p_LineLength, float p_Angle);

	/*!
		\brief Improves circle transform data.
		\param p_CircleTransforms The vector of circle transforms, to improve.
		\return Nothing.
	*/
	void ImproveTransformData(std::vector<CircleTransformData> &p_CircleTransforms);

public:
	Deck() = default;	//!< Constructor.
	~Deck() = default;	//!< Destructor.

	/*!
		\brief Generates the card entities.
		\param p_CardPosition The position of the card.
		\param p_CardRadius The radius of the card.
		\param p_NumberOfSymblesPerCard The number of symbols each card has.
		\return Nothing.
	*/
	void GenerateCards(Vector2Df p_CardPosition, float p_CardRadius, unsigned int p_NumberOfSymblesPerCard);
	/*!
		\brief Shuffles the (deck) card IDs, stored.
		\return Nothing.
	*/
	void Shuffle();

	/*!
		\brief Checks if a render component has the a symbol ID.
		\param p_DeckCardRenderComponent The render component storing the symbols IDs to check.
		\param p_PlayerSymbolIDGuess The symbol ID to check for.
		\return Returns true if the render component has the symbol, otherwise false.
	*/
	static bool HasMatchingSymbol(std::shared_ptr<RenderComponent> p_DeckCardRenderComponent, unsigned int p_PlayerSymbolIDGuess);

	/*!
		\brief Checks if the deck is empty.
		\return Returns true if the deck is empty, otherwise false.
	*/
	bool IsDeckEmpty() const;
	/*!
		\brief Gets the number of remaining cards, in the deck.
		\return Returns the number of remaining card, in the deck.
	*/
	unsigned int NumberOfRemainingCards() const;
	/*!
		\brief Gets the card entity's ID, at the top of the deck, and removes it, from the deck.
		\return Returns the ID of the card entity at the top of the deck, if there's one, otherwise it returns UINT_MAX.
	*/
	unsigned int GetCardIDFromTop();
	/*!
		\brief Gets a vector of the card IDs.
		\return Returns a vector of the deck's card IDs.
	*/
	std::vector<unsigned int> &GetCardIDs();
};