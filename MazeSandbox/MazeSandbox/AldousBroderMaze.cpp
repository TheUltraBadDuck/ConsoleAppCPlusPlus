#include "AldousBroderMaze.h"


void AldousBroderMaze::updateDirectionState() {
	SET_DIRECTION_VALUE(false);
	_up = (curr.Y > 0) and BITMAP_S(curr, 0, -1);
	_left = (curr.X > 0) and BITMAP_S(curr, -1, 0);
	_down = (curr.Y < 15) and BITMAP_S(curr, 0, 1);
	_right = (curr.X < 15) and BITMAP_S(curr, 1, 0);
}


#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
void AldousBroderMaze::generate(Maze& maze, COORD& pos) {

	// 0) First, let's choose the pinned unit
	curr.X = rand() % 16;
	curr.Y = rand() % 16;
	BITMAP(curr) = false;

	char direction = 0;

	short count = 16 * 16 - 1;
	while (count > 0) {

		prev = curr;
		bool adding = false;

		// 1) Check empty neighbours
		updateDirectionState();

		// Then random the valid direction then move
		do {
			direction = rand() % 4;
		} while (jumpingOutside(direction, curr));

		// If head to the empty neighbour: add like Random DFS
		// Else, just jump
		switch (direction) {
		case 0:	curr.Y--; adding = _up; break; // up
		case 1: curr.X--; adding = _left; break; // left
		case 2: curr.Y++; adding = _down; break; // down
		case 3: curr.X++; adding = _right; break; // right
		}

		// Update data
		if (adding) {
			BITMAP(curr) = false;

			Console::setColour(white * 16);
			maze.connect(prev, curr);
			maze.instantlyDrawUnit(prev);
			maze.instantlyDrawEdge(prev, curr);
			maze.instantlyDrawUnit(curr);

			count--;
		}

		Sleep(30);
		maze.drawCursor(prev, curr);
	}

	pos = curr;
	maze.cycling = false;
	maze.drawCursor(pos);
}
