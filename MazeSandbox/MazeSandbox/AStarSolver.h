#ifndef _ASTAR_SOLVER
#define _ASTAR_SOLVER


#include "PathFinding.h"


struct AStarBitmap {
	COORD pos{};
	COORD parent{};
	short g_cost = 0;
	short h_cost = 0;
	AStarBitmap(COORD& pos, COORD& parent,  COORD& start_pos, COORD& finish_pos) {
		this->pos = pos;
		this->parent = parent;
		g_cost = std::abs(start_pos.X - pos.X) + std::abs(start_pos.Y - pos.Y);
		h_cost = std::abs(finish_pos.X - pos.X) + std::abs(finish_pos.Y - pos.Y);
	}
};


class AStarSolver : public PathFinding {

protected:
	std::vector<AStarBitmap> path_list;
	COORD bitmap[16][16];			// I will use the case 'COORD' parent. 

public:
	AStarSolver() : PathFinding() {
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
