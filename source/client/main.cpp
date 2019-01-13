#include <SFML/System.hpp>

#include "Game.h"
#include "Logger.h"

int main(int p_Argc, char* p_Argv[]) {
	Log::Initialise();
	Log::Flush("ALL");

	int width = 1280;
	int height = 720;

	Window window(sf::VideoMode(width, height), "Game Engine", sf::Style::Default);
	Game game(window);
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);

	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	unsigned int updateCounter = 0, renderCounter = 0;
	sf::Time totalTime = sf::Time::Zero;
	while (window.IsOpen()) {
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		totalTime += elapsedTime;

		game.ProcessEvents();

		while (timeSinceLastUpdate >= timePerFrame) {
			timeSinceLastUpdate -= timePerFrame;
			game.Update(timePerFrame.asSeconds());
			//Log(MessageType::INFO) << "Update counter: " << ++updateCounter / totalTime.asSeconds();
		}
		game.Render();
		//Log(MessageType::INFO) << "Render counter: " << ++renderCounter / totalTime.asSeconds();

		window.Display();
	}

	return 0;
}