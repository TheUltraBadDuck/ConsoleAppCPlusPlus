#include "RandomDFSMaze.h"


void RandomDFSMaze::updateDirectionState() {
	SET_DIRECTION_VALUE(false);
	_up		= (curr.Y > 0) and BITMAP_S(curr, 0, -1);
	_left	= (curr.X > 0) and BITMAP_S(curr, -1, 0);
	_down	= (curr.Y < 15) and BITMAP_S(curr, 0, 1);
	_right	= (curr.X < 15) and BITMAP_S(curr, 1, 0);
}


#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
void RandomDFSMaze::generate(Maze& maze, COORD& pos) {

	// 0) First, let's choose the pinned unit
	curr.X = rand() % 16;
	curr.Y = rand() % 16;
	BITMAP(curr) = false;
	pos_stack.push(curr);

	short count = 16 * 16 - 1;
	while (count > 0) {

		while (true) {

			prev = curr;

			// 1) Check empty neighbours around the unit
			updateDirectionState();

			// 1.1) If there are neighbours => jump below
			if (_up or _left or _down or _right)
				break;
			// 1.2) If there is no neighbour => return to the previous unit to check and repeat the step 1)
			else {
				pos_stack.pop();
				curr = pos_stack.top();

				Sleep(30);
				maze.drawCursor(prev, curr);
			}
		}

		// 2) Then random the valid direction then move
			// Choose the random empty neighbour
		char direction;
		do {
			direction = rand() % 4;
		} while (not nearFreeUnit(DIRECTION_GROUP, direction));

		// Set connection between two units
		switch (direction) {
		case 0: curr.Y--; break;	// up
		case 1: curr.X--; break;	// left
		case 2:	curr.Y++; break;	// down
		case 3: curr.X++; break;	// right
		}

		// 3) Update data
		BITMAP(curr) = false;
		pos_stack.push(curr);

		Console::setColour(white * 16);
		maze.connect(prev, curr);
		maze.instantlyDrawUnit(prev);
		maze.instantlyDrawEdge(prev, curr);
		maze.instantlyDrawUnit(curr);

		count--;

		Sleep(30);
		maze.drawCursor(prev, curr);
	}

	pos = curr;
	maze.cycling = false;
}
