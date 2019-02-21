#pragma once

#include <map>
#include <vector>

#include "Server.h"
#include "Deck.h"

class ServerGame {
private:
	static const constexpr unsigned int s_m_NumberOfPlayers = 1;//3;

	Server m_Server;
	Deck m_Deck;
	unsigned int m_ActiveDeckCard;
	bool m_IsRunning;

	void SendStartingInformation();
	bool CheckIfPlayerWonRound(const ClientID &p_ClientID, sf::Packet &p_Packet);

public:
	ServerGame();
	~ServerGame();

	void Update(float p_DeltaTime);

	bool IsRunning() const;
};