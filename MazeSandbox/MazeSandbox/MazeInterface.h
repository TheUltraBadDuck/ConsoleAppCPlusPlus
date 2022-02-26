#ifndef _MAZE_INTERFACE_H
#define _MAZE_INTERFACE_H


#include "RandomDFSMaze.h"
#include "RandomKruskalMaze.h"
#include "RandomPrimMaze.h"
#include "AldousBroderMaze.h"
#include "HuntAndKillMaze.h"
#include "BinaryTreeMaze.h"
#include "EllerMaze.h"

#include "DFSSolver.h"
#include "BFSSolver.h"
#include "DijkstraSolver.h"
#include "AStarSolver.h"

#include "Pledge.h"


class MazeInterface {

private:
	Drawing drawing;
	COORD current_pos{ 0, 0 };		// The current position of the cursor
	bool move_and_draw = false;		// if true, when moving the cursor will trail the road

	Maze maze;

public:
	MazeInterface();
	~MazeInterface();

	void runProject();

	void generateMaze(short&);
	void solveMaze(short&);
};


#endif
