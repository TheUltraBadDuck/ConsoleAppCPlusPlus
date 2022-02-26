#include "DijkstraSolver.h"


void DijkstraSolver::updateDirectionState(Maze& maze) {
	_up		= MAP(curr).up		and BITMAP_S(curr, 0, -1) == -1;
	_left	= MAP(curr).left	and BITMAP_S(curr, -1, 0) == -1;
	_down	= MAP(curr).down	and BITMAP_S(curr, 0, 1) == -1;
	_right	= MAP(curr).right	and BITMAP_S(curr, 1, 0) == -1;
}


void DijkstraSolver::updateCycle(Maze& maze) {
	// Check cycle
	// Remember to check the direction, we do not want to check the previous bitmap
	if (maze.cycling == true)
		return;

	COORD temp{};

	temp.X = curr.X;
	temp.Y = curr.Y - 1;
	if (MAP(curr).up and (BITMAP(temp) != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X - 1;
	temp.Y = curr.Y;
	if (MAP(curr).left and (BITMAP(temp) != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X;
	temp.Y = curr.Y + 1;
	if (MAP(curr).down and (BITMAP(temp) != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}

	temp.X = curr.X + 1;
	temp.Y = curr.Y;
	if (MAP(curr).right and (BITMAP(temp) != -1) and (not equalCoord(prev, temp))) {
		maze.cycling = true;
		return;
	}
}


// Order to get the bitmap
// Choose the bitmap whose g(n) is the smallest
bool gettingPriorityBitmap(DijkstraBitmap& first, DijkstraBitmap& second) {
	return first.g_cost > second.g_cost;
}


char DijkstraSolver::findPath(Maze& maze) {

	maze.winnable = false;
	maze.cycling = false;

	// If there is no start point or finish point => error
	if (maze.start_pos.X == -1)
		return 1;
	if (maze.finish_pos.X == -1)
		return 2;

	// 1) Choose the start position as the starting position
	curr = maze.start_pos;
	// At start point, the bitmap will value 0
	BITMAP(curr) = 0;

	Console::setColour(yellow * 16);

	while (true) {

		// 1) Check true bit around the unit
		updateDirectionState(maze);

		// 1.1) If valid, add them to list
		// Order of choosing position: from up, then anti-clockwisedly left, down and right
		// In order to do that, we must push reversedly
		if (_right) {
			COORD temp = curr;
			temp.X++;
			BITMAP(temp) = BITMAP(curr) + 1;
			path_list.push_back(DijkstraBitmap(temp, curr, BITMAP_S(curr, 1, 0)));
		}
		if (_down) {
			COORD temp = curr;
			temp.Y++;
			BITMAP(temp) = BITMAP(curr) + 1;
			path_list.push_back(DijkstraBitmap(temp, curr, BITMAP_S(curr, 0, 1)));
		}
		if (_left) {
			COORD temp = curr;
			temp.X--;
			BITMAP(temp) = BITMAP(curr) + 1;
			path_list.push_back(DijkstraBitmap(temp, curr, BITMAP_S(curr, -1, 0)));
		}
		if (_up) {
			COORD temp = curr;
			temp.Y--;
			BITMAP(temp) = BITMAP(curr) + 1;
			path_list.push_back(DijkstraBitmap(temp, curr, BITMAP_S(curr, 0, -1)));
		}

		// 1.1.1) If there is no list adding => the start point and finish point do not connect each other
		if (path_list.size() == 0) {
			maze.redrawStartAndFinish();
			return 3;
		}

		// 1.2) Get smallest value then pop it
		std::sort(path_list.begin(), path_list.end(), gettingPriorityBitmap);
		DijkstraBitmap temp = path_list.back();
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
	// If bitmap(curr) == 0 => the road has successfully created
	// Or: each bitmap will save its parent position
	 Console::setColour(dark_yellow * 16);

	while (true) {
		COORD prev = curr;
		short distance = BITMAP(curr) - 1;

		if (MAP(curr).up and (BITMAP_S(curr, 0, -1) == distance))
			curr.Y--;
		else if (MAP(curr).left and (BITMAP_S(curr, -1, 0) == distance))
			curr.X--;
		else if (MAP(curr).down and (BITMAP_S(curr, 0, 1) == distance))
			curr.Y++;
		else if (MAP(curr).right and (BITMAP_S(curr, 1, 0) == distance))
			curr.X++;

		maze.instantlyDrawEdge(prev, curr);
		Sleep(15);
		maze.instantlyDrawUnit(curr);
		Sleep(15);

		if (distance == 0)
			break;
	}

	maze.redrawStartAndFinish();
	return 0;
}
