/**
@file GameState.h
@brief The different game states available.
*/
#pragma once

/**
@enum GameState
@brief The different game states available.
*/

enum class GameState : unsigned int {
	MAIN_MENU = 0,	/**< Main menu scene. */
	ACTIVE,	/**< Currently playing the game. */
	HELP,	/**< Help scene. */
	WIN,	/**< The player has won. */
	LOSE,	/**< The player has lost. */
	SHUTDOWN,	/**< The game should shut down. */

	NOT_AVAILABLE	/**< Invalid state. */
};