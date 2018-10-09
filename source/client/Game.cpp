#include "Game.h"

#include "GLCore.hpp"

Game::Game(Window &p_Window) : m_Window(p_Window) {
	
}

Game::~Game() {

}

void Game::ProcessEvents() {
	sf::Event event;

	while (m_Window.GetWindow().pollEvent(event)) {
		// Send the event to the user interface class, so that can handle it.

		if (event.type == sf::Event::Closed)
			m_Window.GetWindow().close();
	}
}

void Game::Update(float p_DeltaTime) {

}

void Game::Render() {
	if (!m_Window.IsOpen())
		return;

	// Clear the window with grey.
	gl::ClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

	// Switch the buffers.
	m_Window.GetWindow().display();
}