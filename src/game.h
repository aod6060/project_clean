#pragma once


#include <sys.h>
#include "game_states.h"


struct GameStateWindowCallback : public StateWindowCallback {
	Logo1State logo1State;
	Logo2State logo2State;
	StartMenuState startMenuState;
	GameState gameState;

	virtual void init();
};