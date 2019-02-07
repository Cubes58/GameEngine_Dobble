#pragma once

#include "Deck.h"

class ServerGame {
private:
	Deck m_Deck;
	bool m_IsRunning;

public:
	ServerGame();
	~ServerGame();

	void Update(float p_DeltaTime);

	bool IsRunning() const;
};