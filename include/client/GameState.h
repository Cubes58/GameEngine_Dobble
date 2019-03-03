#pragma once

enum class GameState : unsigned int {
	MAIN_MENU = 0,
	ACTIVE,
	HELP,
	WIN,
	LOSE,
	SHUTDOWN,

	NOT_AVAILABLE
};