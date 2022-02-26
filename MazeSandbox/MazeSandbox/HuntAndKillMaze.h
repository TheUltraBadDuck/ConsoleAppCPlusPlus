#ifndef _HUNT_AND_KILL_MAZE
#define _HUNT_AND_KILL_MAZE


#include "MazeGenerator.h"


class HuntAndKillMaze : public MazeGenerator {

protected:
	bool bitmap[16][16];			// Set the value of the virtual map
									// the same as Randomized DFS Maze

	COORD curr{};					// Current position
	COORD prev{};					// Previous position
	bool DIRECTION_GROUP;			// Validity of four directions: if true: free, if false: occupied

public:
	HuntAndKillMaze() : MazeGenerator() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j] = true;
			}
		}

		SET_DIRECTION_VALUE(false);
	}

	void generate(Maze& maze, COORD& pos);

	void updateDirectionState();
	COORD getNearestEmptyNeighbour(Maze& maze);
	bool isNearestEmptyNeighbour(COORD& pos);
	char getDirectionForHunting();

	void drawCyanArrow(short& lane);
	void removeCyanArrow(short& lane);
};


#endif
