#ifndef _RANDOM_KRUSKAL_MAZE
#define _RANDOM_KRUSKAL_MAZE


#include "MazeGenerator.h"


// Use the basic Kruskal theory
struct KruskalUnit {
	COORD parent{};		// If not connect, the unit's parent is itself. If connecting, the unit's parent will change
	short rank = 0;		// Or (repeat: OR) the rank will increase
};


class RandomKruskalMaze : public MazeGenerator {

protected:
	// Total vertices: 16 * 16 = 256
	// Total edges: 16 * (16 - 1) * 2 = 480
	// Number of using edges: 16 * 15 + 15 = 256 - 1 = 255

	KruskalUnit bitmap[16][16];		// See Kruskal's Algorithm for understanding this struct

public:
	RandomKruskalMaze() : MazeGenerator() {
		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {
				bitmap[i][j].parent = COORD{j, i};
			}
		}
	}

	void generate(Maze& maze, COORD& pos);

	COORD findParent(COORD&);

};


#endif
