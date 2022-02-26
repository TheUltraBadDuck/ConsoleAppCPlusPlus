#ifndef _ELLER_MAZE
#define _ELLER_MAZE


#include "MazeGenerator.h"


class EllerMaze : public MazeGenerator {

protected:
	std::map<short, std::vector<short> > pos_storage;					// each bit group will store a certain position
	std::map<short, std::vector<short> > con_above_pos_storage;			// like above but this contains connection above
	std::vector<short> left_wall_poss;									// For creating the walls
	short bitmap[16][16];			// Note this one is using char

public:
	EllerMaze() : MazeGenerator() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j] = -1;
			}
		}
	}

	void generate(Maze& maze, COORD& pos);
	void mergeBitmap(short bit1, short bit2);

};


#endif
