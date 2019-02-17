#pragma once

#include <map>

#include "Server.h"
#include "Deck.h"

class ServerGame {
private:
	Server m_Server;
	Deck m_Deck;
	bool m_IsRunning;

public:
	ServerGame();
	~ServerGame();

	void Update(float p_DeltaTime);

	bool IsRunning() const;
};