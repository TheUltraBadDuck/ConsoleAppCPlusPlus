#ifndef _DIJKSTRA_SOLVER
#define _DIJKSTRA_SOLVER


#include "PathFinding.h"


// If you look into details, this is almost the same as BFS Pathfinding!


struct DijkstraBitmap {
	COORD pos{};
	COORD parent{};
	short g_cost = 0;
	DijkstraBitmap(COORD& pos, COORD& parent, short g_cost) {
		this->parent = parent;
		this->pos = pos;
		this->g_cost = g_cost;
	}
	DijkstraBitmap(COORD& pos, COORD& parent, COORD& start_pos) {
		this->parent = parent;
		this->pos = pos;
		g_cost = std::abs(start_pos.X - pos.X) + std::abs(start_pos.Y - pos.Y);
	}	// Both way will receive the same g(x) value
};


class DijkstraSolver : public PathFinding {

protected:
	std::vector<DijkstraBitmap> path_list;
	short bitmap[16][16];			// Store 'short' g_cost
									// You can store 'COORD' parent instead. In that case, the trace from finish point to start point will change
									// If you want to see the 'COORD' parent instance, see AStarSolver

public:
	DijkstraSolver() : PathFinding() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j] = -1;
			}
		}
	}

	char findPath(Maze& maze);
	void updateCycle(Maze& maze);
	void updateDirectionState(Maze& maze);
};


#endif
