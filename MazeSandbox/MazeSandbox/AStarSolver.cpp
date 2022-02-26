#include "AStarSolver.h"


void AStarSolver::updateDirectionState(Maze& maze) {
	_up		= MAP(curr).up		and (BITMAP_S(curr, 0, -1).X == -1);
	_left	= MAP(curr).left	and (BITMAP_S(curr, -1, 0).X == -1);
	_down	= MAP(curr).down	and (BITMAP_S(curr, 0, 1).X == -1);
	_right	= MAP(curr).right	and (BITMAP_S(curr, 1, 0).X == -1);
}


void AStarSolver::updateCycle(Maze& maze) {
	// Check cycle
	// Remember to check the direction, we do not want to check the previous bitmap
	if (maze.cycling == true)
		return;

	COORD temp{};

	temp.X = curr.X;
	temp.Y = curr.Y - 1;
	if (MAP(curr).up and (BITMAP(temp).X != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X - 1;
	temp.Y = curr.Y;
	if (MAP(curr).left and (BITMAP(temp).X != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X;
	temp.Y = curr.Y + 1;
	if (MAP(curr).down and (BITMAP(temp).X != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X + 1;
	temp.Y = curr.Y;
	if (MAP(curr).right and (BITMAP(temp).X != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}
}


// Order to get the bitmap
// 1) Choose the bitmap whose f(x) = g(x) + h(x) is the smallest
// 2) If both are equal, choose the shortest h(x)		(Without this, the way will add the 'digital signal' if there are wider roads)
bool gettingPriorityBitmap(AStarBitmap& first, AStarBitmap& second) {
	if ((first.g_cost + first.h_cost) == (second.g_cost + second.h_cost))
		return first.h_cost > second.h_cost;
		
	return (first.g_cost + first.h_cost) > (second.g_cost + second.h_cost);
}


char AStarSolver::findPath(Maze& maze) {

	maze.winnable = false;
	maze.cycling = false;

	// If there is no start point or finish point => error
	if (maze.start_pos.X == -1)
		return 1;
	if (maze.finish_pos.X == -1)
		return 2;

	// 1) Choose the start position as the starting position
	curr = maze.start_pos;
	// At start point, the bitmap will point to zero
	BITMAP(curr) = curr;

	Console::setColour(yellow * 16);

	while (true) {

		// 1) Check true bit around the unit
		updateDirectionState(maze);

		// 1.1) If valid, add them to list
		if (_right) {
			COORD temp = curr;
			temp.X++;
			BITMAP(temp) = curr;
			path_list.push_back(AStarBitmap(temp, curr, maze.start_pos, maze.finish_pos));
		}
		if (_down) {
			COORD temp = curr;
			temp.Y++;
			BITMAP(temp) = curr;
			path_list.push_back(AStarBitmap(temp, curr, maze.start_pos, maze.finish_pos));
		}
		if (_left) {
			COORD temp = curr;
			temp.X--;
			BITMAP(temp) = curr;
			path_list.push_back(AStarBitmap(temp, curr, maze.start_pos, maze.finish_pos));
		}
		if (_up) {
			COORD temp = curr;
			temp.Y--;
			BITMAP(temp) = curr;
			path_list.push_back(AStarBitmap(temp, curr, maze.start_pos, maze.finish_pos));
		}

		// 1.1.1) If there is no list adding => the start point and finish point do not connect each other
		if (path_list.size() == 0) {
			maze.redrawStartAndFinish();
			return 3;
		}

		// 1.2) Get smallest value then pop it
		// The difference between Dijkstra's Searching Algorithm and A Star Searching one is here
		// A Star one include heuristic value (h_cost)
		std::sort(path_list.begin(), path_list.end(), gettingPriorityBitmap);
		AStarBitmap temp = path_list.back();
		path_list.pop_back();

		prev = temp.parent;
		curr = temp.pos;

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
