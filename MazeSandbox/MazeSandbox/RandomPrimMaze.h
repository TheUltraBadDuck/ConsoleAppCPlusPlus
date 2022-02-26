#ifndef _RANDOM_PRIM_MAZE
#define _RANDOM_PRIM_MAZE

#include "MazeGenerator.h"


enum class BitmapState {
	empty,	// the unit has not been chosen yet
	ready,	// the unit is the neighbour, going to be chosen soon
	occupied	// the unit has been taken
};


class RandomPrimMaze : public MazeGenerator {

protected:
	std::vector<COORD> pos_list;	// to get random element and dynamic storage
	BitmapState bitmap[16][16];		// Set the value of the virtual map

	COORD curr{};					// Current position
	COORD prev{};					// Taken position
	bool DIRECTION_GROUP;			// Validity of four directions: if true: free, if false: occupied

public:
	RandomPrimMaze() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j] = BitmapState::empty;
			}
		}

		SET_DIRECTION_VALUE(false);
	}

	void generate(Maze& maze, COORD& pos);

	void updateDirectionState();

	void setNeighbourList(COORD&);
};


#endif
