#include "DFSSolver.h"


void DFSSolver::updateDirectionState(Maze& maze) {
	_up		= MAP(curr).up		and BITMAP_S(curr, 0, -1);
	_left	= MAP(curr).left	and BITMAP_S(curr, -1, 0);
	_down	= MAP(curr).down	and BITMAP_S(curr, 0, 1);
	_right	= MAP(curr).right	and BITMAP_S(curr, 1, 0);
	crossroad = (char)_up + (char)_left + (char)_down + (char)_right;
}


void DFSSolver::updateCycle(Maze& maze) {
	// Check cycle
	// Remember to check the direction, we do not want to check the previous bitmap
	if (maze.cycling == true)
		return;

	COORD temp{};

	temp.X = curr.X;
	temp.Y = curr.Y - 1;
	if (MAP(curr).up and (not BITMAP(temp)) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X - 1;
	temp.Y = curr.Y;
	if (MAP(curr).left and (not BITMAP(temp)) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X;
	temp.Y = curr.Y + 1;
	if (MAP(curr).down and (not BITMAP(temp)) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X + 1;
	temp.Y = curr.Y;
	if (MAP(curr).right and (not BITMAP(temp)) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}
}


char DFSSolver::findPath(Maze& maze) {

	maze.winnable = false;

	// If there is no start point or finish point => error
	if (maze.start_pos.X == -1)
		return 1;
	if (maze.finish_pos.X == -1)
		return 2;

	// 1) Choose the start position as the starting position
	curr = maze.start_pos;
	BITMAP(curr) = false;
	final_way.push(curr);

	Console::setColour(yellow * 16);

	while (true) {

		prev = curr;

		// 1) Check true bit around the unit
		updateDirectionState(maze);

		// 1.1.1) If the crossroad is larger than or equal 2 => push the stack
		// 1.1.2) If there are just one => choose the valid unit to jump
		if (crossroad > 0) {
			for (short i = 0; i < crossroad - 1; i++) {
				pos_stack.push(curr);
			}
		}
		// 1.1.3) If there are none => return to the previous unit to check and repeat the step 1)
		else {
			if (pos_stack.size() > 0) {
				curr = pos_stack.top();
				pos_stack.pop();

				while ((final_way.size() > 0) and not equalCoord(final_way.top(), curr))
					final_way.pop();
				continue;
			}
			else {		// There is a case where the maze is impossible to connect between the start point and finish point. In this case, the game is error
				maze.redrawStartAndFinish();
				return 3;
			}
		}

		// 1.2) Order of choosing position: from up, then anti-clockwisedly left, down and right
		if (_up)
			curr.Y--;
		else if (_left)
			curr.X--;
		else if (_down)
			curr.Y++;
		else
			curr.X++;

		BITMAP(curr) = false;
		final_way.push(curr);

		// Check cycle
		updateCycle(maze);

		maze.instantlyDrawEdge(prev, curr);
		Sleep(30);
		maze.instantlyDrawUnit(curr);
		Sleep(30);

		if (equalCoord(curr, maze.finish_pos)) {
			maze.winnable = true;
			break;
		}
	}

	maze.redrawStartAndFinish();

	// Draw the final way
	Console::setColour(dark_yellow * 16);
	while (final_way.size() > 0) {
		COORD prev = curr;
		curr = final_way.top();
		final_way.pop();
		maze.instantlyDrawEdge(prev, curr);
		Sleep(15);
		maze.instantlyDrawUnit(curr);
		Sleep(15);
	}

	maze.redrawStartAndFinish();
	return 0;
}
