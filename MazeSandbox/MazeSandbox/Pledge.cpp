#include "Pledge.h"


char Pledge::getDirectionTowardFinish(Maze& maze) {

	COORD start_finish{};
	start_finish.X = maze.finish_pos.X - maze.start_pos.X;
	start_finish.Y = maze.finish_pos.Y - maze.start_pos.Y;

	if (abs(start_finish.Y) > abs(start_finish.X)) {
		if (start_finish.Y < 0)
			return '^';
		else
			return 'v';
	}
	else {
		if (start_finish.X < 0)
			return '<';
		else
			return '>';
	}
}


char Pledge::findPath(Maze& maze) {

	maze.winnable = false;
	maze.cycling = false;

	// If there is no start point or finish point => error
	if (maze.start_pos.X == -1)
		return 1;
	if (maze.finish_pos.X == -1)
		return 2;

	// For outputing extended ASCII characters
	SetConsoleOutputCP(437);

	curr = maze.start_pos;
	direction = getDirectionTowardFinish(maze);

	Console::setColour(dark_yellow * 16);

	short pass_start_point_times = 1;
	short count = 16 * 16 * 3;
	while (count > 0) {

		prev = curr;

		// Extra step: If the degree is zero, go foward until it touches the wall
		if (left_follow) {
			if (rotation == 0) {
				if (not haveWallAhead(maze)) {
					moveAhead();
				}
				// For rotation = 0 => always turns right
				else {
					direction = getRightSide(direction);
					rotation -= 90;
					continue;
				}
			}

			else {
				if (not haveWallOnLeft(maze)) {
					direction = getLeftSide(direction);
					rotation += 90;
					moveAhead();
				}
				else if (not haveWallAhead(maze)) {
					moveAhead();
				}
				else {
					direction = getRightSide(direction);
					rotation -= 90;
					continue;
				}
			}
		}
		else {
			if (rotation == 0) {
				if (not haveWallAhead(maze)) {
					moveAhead();
				}
				// For rotation = 0 => always turns left
				else {
					direction = getLeftSide(direction);
					rotation += 90;
					continue;
				}
			}
			else {
				if (not haveWallOnRight(maze)) {
					direction = getRightSide(direction);
					rotation -= 90;
					moveAhead();
				}
				else if (not haveWallAhead(maze)) {
					moveAhead();
				}
				else {
					direction = getLeftSide(direction);
					rotation += 90;
					continue;
				}
			}
		}

		setAndDrawFollowingWay();

		if (equalCoord(curr, maze.start_pos))
			pass_start_point_times++;
		count--;

		if (pass_start_point_times > 4) {
			maze.cycling = true;
			break;
		}

		if (equalCoord(curr, maze.finish_pos)) {
			maze.winnable = true;
			break;
		}
	}

	SetConsoleOutputCP(CP_UTF8);

	maze.redrawStartAndFinish();
	if (maze.winnable)
		return 0;
	else if (maze.cycling)
		return 4;
	else
		return 3;
}
