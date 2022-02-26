#ifndef _MAZE_GENERATOR_H
#define _MAZE_GENERATOR_H


#include "Maze.h"


class MazeGenerator {

public:

	virtual void generate(Maze& maze, COORD& pos) = 0;

};


#endif
