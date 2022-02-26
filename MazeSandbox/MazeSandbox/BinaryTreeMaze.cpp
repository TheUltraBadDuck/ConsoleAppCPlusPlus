#include "BinaryTreeMaze.h"


void BinaryTreeMaze::generate(Maze& maze, COORD& pos) {

	// 0) Choose the left up position as the start point
	maze.drawCursor(curr);
	Sleep(30);

	// 0.1) At row 0: connect all of them
	for (curr.X = 1; curr.X < 16; curr.X++) {

		Console::setColour(white * 16);
		maze.connect(prev, curr);
		maze.instantlyDrawUnit(prev);
		maze.instantlyDrawEdge(prev, curr);
		maze.instantlyDrawUnit(curr);

		maze.drawCursor(prev, curr);
		Sleep(30);

		prev = curr;
	}

	COORD temp{};
	// 1) The rest: connect either the left or up unit
	for (curr.Y = 1; curr.Y < 16; curr.Y++) {

		// 1.1) At column 0: choose the upward unit to connect
		curr.X = 0;
		temp = curr;
		temp.Y--;
		
		Console::setColour(white * 16);
		maze.connect(temp, curr);
		maze.instantlyDrawUnit(temp);
		maze.instantlyDrawEdge(temp, curr);
		maze.instantlyDrawUnit(curr);

		Sleep(30);
		maze.drawCursor(prev, curr);

		prev = curr;

		// 1.2) Do as step 1)
		for (curr.X = 1; curr.X < 16; curr.X++) {

			temp = curr;
			switch (rand() % 2) {
			case 0:		// up
				temp.Y--;
				break;
			case 1:		// left
				temp.X--;
				break;
			}

			Console::setColour(white * 16);
			maze.connect(temp, curr);
			maze.instantlyDrawUnit(temp);
			maze.instantlyDrawEdge(temp, curr);
			maze.instantlyDrawUnit(curr);

			maze.drawCursor(prev, curr);
			Sleep(30);

			prev = curr;
		}
	}
	curr.X = 15;
	curr.Y = 15;
	maze.cycling = false;
	maze.drawCursor(curr, pos);
}
