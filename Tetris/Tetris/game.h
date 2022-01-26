#ifndef _GAME_
#define _GAME_


#include "t_block.h"
#include "l_block.h"
#include "j_block.h"
#include "i_block.h"
#include "s_block.h"
#include "z_block.h"
#include "o_block.h"


class Game {

private:
	// Value
	Level level;
	Score score;
	short requiring_bar;				// need to get 12 full lane to jump to next level
	std::string time;					// delay = 100, sleep = 10 => delay * sleep = 1 second
	short delay; short running_delay;

	// Status
	bool exit_game;

	// other useful things
	std::queue<BlockName> waiting_blocks;	// Waiting blocks for the 'next' frame

	BlockName held_block;				// The block for the 'hold' frame
	bool holdable;

public:
	Game();
	~Game();

	void drawGame();			// Draw the interface of the game


	// setter and getter
	void setInitialValue();
	void setSavedValue();

	void setBlock(Tetromino*& new_tetromino);		// Set the random block
		BlockName getRandomType();
	void setWaitingBlocks();
	void setRequiringBar();
	void setHeldBlock(Tetromino*& new_tetromino);		// Hold the block

	// Draw the score, level
	void drawLevel();
	void drawScore();
	void drawTime();
	void drawHeldBlock();
	void drawWaitingBlocks();
	void drawRequiringBar();
	void redrawGame();			// For checkFullLane and fixing bug
	
	// Set the game
	void runGame();				// Main program using while loop
	void checkFullLane(Tetromino*& new_tetromino);		// If one of the lanes are full






	// Mix functions
	void resetBlock(Tetromino*& new_tetromino);

};


#endif
