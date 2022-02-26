#ifndef _PATHFINDING_H
#define _PATHFINDING_H


#include "Maze.h"


class PathFinding {

protected:
	COORD curr{};
	COORD prev{};
	bool DIRECTION_GROUP;

public:
	PathFinding() {
		SET_DIRECTION_VALUE(false);
	}

	virtual char findPath(Maze& maze) = 0;
	virtual void updateCycle(Maze& maze) = 0;
	virtual void updateDirectionState(Maze& maze) = 0;
};


#endif