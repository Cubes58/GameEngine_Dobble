#include <SFML\System.hpp>

#include "Game.h"

int main() {
	Log::Clear();

	int width = 1280;
	int height = 720;

	Window window(sf::VideoMode(width, height), "Game Engine", sf::Style::Default);
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