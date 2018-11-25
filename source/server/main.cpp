#include <SFML/System.hpp>

#include "Logger.h"
#include "ServerGame.h"

int main(int p_Argc, char* p_Argv[]) {
	Log::Clear();

	ServerGame serverGame;
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (serverGame.IsRunning()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			serverGame.Update(timePerFrame.asSeconds());
		}
	}

	return 0;
}