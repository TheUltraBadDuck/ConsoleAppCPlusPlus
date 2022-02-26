#ifndef _BFS_SOLVER
#define _BFS_SOLVER


#include "PathFinding.h"


// If you notice, the Dijsktra's algorithm is quite the same as 


class BFSSolver : public PathFinding {

protected:
	std::queue<COORD> pos_queue;	// FIFO data structure
	std::queue<char> dir_queue;
	COORD bitmap[16][16];			// This will be familiar with DFS, if you know

public:
	BFSSolver() : PathFinding() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j].X = -1;
				bitmap[i][j].Y = -1;
			}
		}
	}

	char findPath(Maze& maze);
	void updateCycle(Maze& maze);
	void updateDirectionState(Maze& maze);
};


#endif
