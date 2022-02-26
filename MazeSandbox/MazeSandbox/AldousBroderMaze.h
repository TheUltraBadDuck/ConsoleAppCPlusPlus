#ifndef _ALDOUS_BRODER_MAZE
#define _ALDOUS_BRODER_MAZE


#include "MazeGenerator.h"


class AldousBroderMaze : public MazeGenerator {

protected:

	bool bitmap[16][16];			// Set the value of the virtual map
									// the same as Randomized DFS Maze

	COORD curr{};					// Current position
	COORD prev{};					// Previous position
	bool DIRECTION_GROUP;			// Validity of four directions: if true: free, if false: occupied

public:
	AldousBroderMaze() : MazeGenerator() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j] = true;
			}
		}

		SET_DIRECTION_VALUE(false);
	}

	void generate(Maze& maze, COORD& pos);

	void updateDirectionState();
};


#endif
