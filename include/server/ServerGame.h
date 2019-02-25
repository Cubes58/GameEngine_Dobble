#pragma once

#include <map>
#include <vector>

#include "Server.h"
#include "Deck.h"

#define SCORE_GAINED_PER_GUESS 25.0f

class ServerGame {
private:
	static const constexpr unsigned int s_m_NumberOfPlayers = 1;//3;

	Server m_Server;
	Deck m_Deck;
	std::map<ClientID, float> m_PlayerScore;
	unsigned int m_ActiveDeckCard;
	bool m_IsRunning;

	void SendStartingInformation();
	void HandlePackets(std::map<ClientID, sf::Packet> &p_Data);
	bool CheckIfPlayerWonRound(const ClientID &p_ClientID, sf::Packet &p_Packet);

public:
	ServerGame();
	~ServerGame();

	void Update(float p_DeltaTime);

	bool IsRunning() const;
};