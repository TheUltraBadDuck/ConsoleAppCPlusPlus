#ifndef _DFS_SOLVER
#define _DFS_SOLVER


#include "PathFinding.h"


class DFSSolver : public PathFinding {

protected:
	std::stack<COORD> pos_stack;	// FILO data structure
	bool bitmap[16][16];			// You will see the familiar between this and RandomDFSMaze
	std::stack<COORD> final_way;	// Useful for drawing the single path

	char crossroad = 0;

public:
	DFSSolver() : PathFinding() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j] = true;
			}
		}
	}

	char findPath(Maze& maze);
	void updateCycle(Maze& maze);
	void updateDirectionState(Maze& maze);
};


#endif
