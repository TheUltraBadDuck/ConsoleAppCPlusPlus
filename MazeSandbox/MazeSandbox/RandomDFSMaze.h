#ifndef _RANDOM_DFS_MAZE
#define _RANDOM_DFS_MAZE


#include "MazeGenerator.h"


class RandomDFSMaze : public MazeGenerator {

protected:
	std::stack<COORD> pos_stack;	// FILO data structure
	bool bitmap[16][16];			// Set the value of the virtual map
									// if false, the unit has been taken

	// Parameters for supporting above
	COORD curr{};					// Current position
	COORD prev{};					// Previous position
	bool DIRECTION_GROUP;			// Validity of four directions: if true: free, if false: occupied

public:
	RandomDFSMaze() : MazeGenerator() {
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
