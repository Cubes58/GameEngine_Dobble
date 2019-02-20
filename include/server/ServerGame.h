#pragma once

#include <map>

#include "Server.h"
#include "Deck.h"

class ServerGame {
private:
	static const constexpr unsigned int s_m_NumberOfPlayers = 3;

	Server m_Server;
	Deck m_Deck;
	bool m_IsRunning;

	void SendStartingInformation();

public:
	ServerGame();
	~ServerGame();

	void Update(float p_DeltaTime);

	bool IsRunning() const;
};