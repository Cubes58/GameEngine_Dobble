/**
@file ServerGame.h
@brief A class handles the server game.
*/
#pragma once

#include <map>
#include <unordered_map>

#include "Server.h"
#include "Deck.h"

#define SCORE_GAINED_PER_GUESS 25.0f	//!< The score a player earns per correct guess.

/*! \class ServerGame
	\brief A class handles the server game.
*/

class ServerGame {
private:
	static const constexpr unsigned int s_m_NumberOfPlayers = 1;	//!< The minimum number of players, per game.

	Server m_Server;	//!< A server instance, which handles the communication between the server and clients.
	Deck m_Deck;	//!< Generates the cards, and helps control the entites.
	std::map<ClientID, float> m_PlayerScores;	//!< Keeps track of the players' scores.
	unsigned int m_ActiveDeckCard;	//!< ID of the current deck card. 
	bool m_GameOver = false;	//!< Keeps track of the game state, whether it's over or not.
	bool m_IsRunning;	//!< Keeps track of the server state, whether it's running or not.

	/*!
		\brief Sends the starting information to the players (their starting card and the deck card).
		\return Nothing.
	*/
	void SendStartingInformation();

	/*!
		\brief Handles incoming data, from the players.
		\param p_Data The data received from the players.
		\return Nothing.
	*/
	void HandlePackets(std::map<ClientID, sf::Packet> &p_Data);

	/*!
		\brief Handles whether a round is won.
		\param p_NumberOfPlayersWonRound the number of players who have won.
		\param p_PlayerRoundState the states of each player that round, whether they've won or not.
		\return Nothing.
	*/
	void HandleRoundWon(int p_NumberOfPlayersWonRound, std::unordered_map<ClientID, bool> &p_PlayerRoundState);

	/*!
		\brief Handles what happens if the game is over, or not, after a round.
		\return Nothing.
	*/
	void HandleGameOver();

	/*!
		\brief Checks if a player have won the game, and what happens if they have.
		\param p_ClientID the client's ID that's being checked.
		\param p_Packet the packet received, from the player (their guess).
		\return Returns true if the player has won, false otherwise.
	*/
	bool CheckIfPlayerWonRound(const ClientID &p_ClientID, sf::Packet &p_Packet);

public:
	/*!
		\brief Constructor.
	*/
	ServerGame();

	/*!
		\brief Destructor.
	*/
	~ServerGame();

	/*!
		\brief Updates the server's game.
		\param p_DeltaTime the delta time between this frame and the last.
		\return Nothing.
	*/
	void Update(float p_DeltaTime);

	/*!
		\brief Returns the state of the server game.
		\return Returns true if the server game is running, false otherwise.
	*/
	bool IsRunning() const;
};