#ifndef _BINARY_TREE
#define _BINARY_TREE


#include "MazeGenerator.h"


class BinaryTreeMaze : public MazeGenerator {

protected:
	COORD curr{};					// Current position
	COORD prev{};					// Previous position

public:
	void generate(Maze& maze, COORD& pos);
};


#endif
