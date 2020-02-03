#pragma once


#include <sys.h>
#include "game_states.h"


struct GameWindowCallback : public StateWindowCallback {
	Logo1State logo1State;
	Logo2State logo2State;
	StartMenuState startMenuState;
	//GameState gameState;
	//ClassModeGameState classModeGameState;
	GameState gameState;

	virtual void init();

	virtual void release();

};