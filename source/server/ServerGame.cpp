#include "ServerGame.h"

ServerGame::ServerGame() : m_IsRunning(true) {

}

ServerGame::~ServerGame() {

}

void ServerGame::Update(float p_DeltaTime) {

}

bool ServerGame::IsRunning() const {
	return m_IsRunning;
}