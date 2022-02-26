#include "HuntAndKillMaze.h"


bool HuntAndKillMaze::isNearestEmptyNeighbour(COORD& pos) {

	if (not bitmap[pos.Y][pos.X])
		return false;

	if ((pos.Y > 0) and (not BITMAP_S(pos, 0, -1)))
		return true;
	if ((pos.X > 0) and (not BITMAP_S(pos, -1, 0)))
		return true;
	if ((pos.Y < 15) and (not BITMAP_S(pos, 0, 1)))
		return true;
	if ((pos.X < 15) and (not BITMAP_S(pos, 1, 0)))
		return true;
	return false;
}

char HuntAndKillMaze::getDirectionForHunting() {
	char direction;

	do {
		direction = rand() % 4;
	} while (
		jumpingOutside(direction, curr) or				// This one is for NOT connecting the border
		nearFreeUnit(DIRECTION_GROUP, direction)		// This one is for connecting the TAKEN unit
		);

	return direction;
}


void HuntAndKillMaze::updateDirectionState() {
	SET_DIRECTION_VALUE(false);
	_up		= (curr.Y > 0) and BITMAP_S(curr, 0, -1);
	_left	= (curr.X > 0) and BITMAP_S(curr, -1, 0);
	_down	= (curr.Y < 15) and BITMAP_S(curr, 0, 1);
	_right	= (curr.X < 15) and BITMAP_S(curr, 1, 0);
}


void HuntAndKillMaze::drawCyanArrow(short& lane) {

	COORD real_pos = { LEFT_BORDER - 4, UP_BORDER + lane * 2 };
	Console::setColour(cyan);
	Console::gotoXY(real_pos);
	std::cout << "=>";
}

void HuntAndKillMaze::removeCyanArrow(short& lane) {

	COORD real_pos = { LEFT_BORDER - 4, UP_BORDER + lane * 2 };
	Console::setColour(white);
	Console::gotoXY(real_pos);
	std::cout << "  ";
}


COORD HuntAndKillMaze::getNearestEmptyNeighbour(Maze& maze) {

	COORD curr{};
	short lane = 0;

	for (curr.Y = 0; curr.Y < 16; curr.Y++) {

		Sleep(60);

		// Draw cyan line
		drawCyanArrow(curr.Y);

		// Remove cyan line
		if (curr.Y != 0) {
			removeCyanArrow(lane);
			lane++;
		}

		for (curr.X = 0; curr.X < 16; curr.X++) {
			if (isNearestEmptyNeighbour(curr)) {
				removeCyanArrow(lane);
				return curr;
			}
		}
	}
	removeCyanArrow(lane);
	return curr;	// if no empty neighbour is found, the return value is (16, 16)
}


#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
void HuntAndKillMaze::generate(Maze& maze, COORD& pos) {

	// 0) First, let's choose the pinned unit
	curr.X = rand() % 16;
	curr.Y = rand() % 16;
	BITMAP(curr) = false;

	short count = 16 * 16 - 1;
	while (count > 0) {

		COORD prev = curr;
		char direction;

		// 1) Check empty neighbours
		updateDirectionState();

		// 1.1) If there is no neighbour => traverse the whole map until we find one empty unit
		if (not (_up or _left or _down or _right)) {

			maze.deleteCursor(curr);
			curr = getNearestEmptyNeighbour(maze);
			if ((curr.X == 16) and (curr.Y == 16))	// Break
				break;

			COORD temp = curr;

			updateDirectionState();
			direction = getDirectionForHunting();

			switch (direction) {
			case 0: temp.Y--; break;	// up
			case 1: temp.X--; break;	// left
			case 2:	temp.Y++; break;	// down
			case 3: temp.X++; break;	// right
			}

			BITMAP(curr) = false;

			Console::setColour(white * 16);
			maze.connect(temp, curr);
			maze.instantlyDrawUnit(temp);
			maze.instantlyDrawEdge(temp, curr);
			maze.instantlyDrawUnit(curr);

			count--;

			continue;
		}

		// 1.2) If there are neighbours => random the valid direction then move
		// 2) Choose the empty neighbor
		do {
			direction = rand() % 4;
		} while (not nearFreeUnit(DIRECTION_GROUP, direction));

		switch (direction) {
		case 0: curr.Y--; break;	// up
		case 1: curr.X--; break;	// left
		case 2:	curr.Y++; break;	// down
		case 3: curr.X++; break;	// right
		}

		// Update data
		BITMAP(curr) = false;

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
	maze.drawCursor(pos);
}
