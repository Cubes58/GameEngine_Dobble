#include <SFML/System.hpp>

#include "Game.h"
#include "Logger.h"

int main(int p_Argc, char* p_Argv[]) {
	Log::Initialise();
	Log::Flush("ALL");

	int width = 1280;
	int height = 720;

	Window window(sf::VideoMode(width, height), "Dobble", sf::Style::Close);
	Game game(window);
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (window.IsOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;

		game.ProcessEvents();
		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			game.Update(timePerFrame.asSeconds());
		}

		game.Render();
		window.Display();
	}

	return 0;
}