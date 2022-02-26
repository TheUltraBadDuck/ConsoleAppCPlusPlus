#include "BFSSolver.h"


void BFSSolver::updateDirectionState(Maze& maze) {
	COORD temp{ -1, -1 };	// If equal, the current position does not have a parent yet
	bool get_up		= equalCoord(BITMAP_S(curr, 0, -1), temp);
	bool get_left	= equalCoord(BITMAP_S(curr, -1, 0), temp);
	bool get_down	= equalCoord(BITMAP_S(curr, 0, 1), temp);
	bool get_right	= equalCoord(BITMAP_S(curr, 1, 0), temp);

	_up		= MAP(curr).up		and get_up;
	_left	= MAP(curr).left	and get_left;
	_down	= MAP(curr).down	and get_down;
	_right	= MAP(curr).right	and get_right;
}


void BFSSolver::updateCycle(Maze& maze) {
	maze.cycling = true;
}


char BFSSolver::findPath(Maze& maze) {

	maze.winnable = false;
	maze.cycling = false;

	// If there is no start point or finish point => error
	if (maze.start_pos.X == -1)
		return 1;
	if (maze.finish_pos.X == -1)
		return 2;

	// 1) Choose the start position as the starting position
	curr = maze.start_pos;
	// At start point, the bitmap will point itself
	BITMAP(curr) = curr;

	Console::setColour(yellow * 16);

	while (true) {

		//for (short i = 0; i < 16; i++) {
		//	for (short j = 0; j < 16; j++) {
		//		Console::gotoXY(j * 3, i);
		//		std::cout << bitmap[i][j].X << ' ';
		//	}
		//}
		//(void)_getch();

		// 1) Check true bit around the unit
		updateDirectionState(maze);

		// 1.1) If valid, add them to FIFO
		// Order of choosing position: from up, then anti-clockwisedly left, down and right
		if (_up) {
			pos_queue.push(curr);
			dir_queue.push('^');
		}
		if (_left) {
			pos_queue.push(curr);
			dir_queue.push('<');
		}
		if (_down) {
			pos_queue.push(curr);
			dir_queue.push('v');
		}
		if (_right) {
			pos_queue.push(curr);
			dir_queue.push('>');
		}
		// 1.1.1) If there is no stack adding => the start point and finish point do not connect each other
		if (pos_queue.size() == 0) {
			maze.redrawStartAndFinish();
			return 3;
		}

		// 1.2) Get the front value and them pop it
		curr = pos_queue.front();
		pos_queue.pop();

		prev = curr;
		switch (dir_queue.front()) {
		case '^': curr.Y--; break;
		case '<': curr.X--; break;
		case 'v': curr.Y++; break;
		case '>': curr.X++; break;
		}
		dir_queue.pop();

		// Update cycle
		// If the bitmap has a parent => jump to other one
		if (BITMAP(curr).X != -1) {
			updateDirectionState(maze);
			continue;
		}

		BITMAP(curr) = prev;

		maze.instantlyDrawEdge(prev, curr);
		Sleep(30);
		maze.instantlyDrawUnit(curr);
		Sleep(30);
		//(void)_getch();

		if (equalCoord(curr, maze.finish_pos)) {
			maze.winnable = true;
			break;
		}
	}

	maze.redrawStartAndFinish();

	// Draw the final way
	Console::setColour(dark_yellow * 16);

	while (true) {
		COORD prev = curr;
		curr = BITMAP(curr);

		if (equalCoord(prev, curr))
			break;

		maze.instantlyDrawEdge(prev, curr);
		Sleep(15);
		maze.instantlyDrawUnit(curr);
		Sleep(15);
	}

	maze.redrawStartAndFinish();
	return 0;
}
