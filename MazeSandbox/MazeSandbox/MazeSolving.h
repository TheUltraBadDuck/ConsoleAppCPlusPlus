#ifndef _MAZE_SOLVING_H
#define _MAZE_SOLVING_H


#include "Maze.h"


class MazeSolving {

protected:
	COORD curr{};
	COORD prev{};
	char direction = 0;

public:
	MazeSolving() {}

	virtual char findPath(Maze& maze) = 0;

};


#endif
