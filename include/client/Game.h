/**
@file Game.h
@brief A class handles the overall game.
*/
#pragma once

#include <memory>

#include "Window.h"
#include "GameState.h"

class Scene;

/*! \class Game
	\brief A class handles the overall game.
*/

class Game {
private:
	Window &m_Window;	//!< A reference to the game's window.
	std::unique_ptr<Scene> m_Scene;	//!< A pointer to the active scene.
	GameState m_GameState;	//!< The game's current state.
	GameState m_PreviousState;	//!< The game's previous state.

	/*!
		\brief Sets the game's scene.
		\return Nothing.
	*/
	void SetScene();

public:
	/*!
		\brief Constructor.
	*/
	Game(Window &p_Window);
	/*!
		\brief Destructor.
	*/
	~Game();

	/*!
		\brief Processes the incoming events.
		\return Nothing.
	*/
	void ProcessEvents();
	/*!
		\brief Updates the game.
		\return Nothing.
	*/
	void Update(float p_DeltaTime);
	/*!
		\brief Renders the game.
		\return Nothing.
	*/
	void Render();
};