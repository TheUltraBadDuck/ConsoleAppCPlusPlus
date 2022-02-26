#include "WallFollowing.h"


// Or get direction in counterclockwise
char getLeftSide(char& direction) {
	switch (direction) {
	case '^': return '<'; break;
	case '<': return 'v'; break;
	case 'v': return '>'; break;
	case '>': return '^'; break;
	default: return 0; break;
	}
}


// Or get direction in clockwise
char getRightSide(char& direction) {
	switch (direction) {
	case '^': return '>'; break;
	case '<': return '^'; break;
	case 'v': return '<'; break;
	case '>': return 'v'; break;
	default: return 0; break;
	}
}


// Since it's Console app, we will use char 220 for bottom-half block
unsigned char BOTTOM_HALF_BLOCK = 220;

#pragma warning(push)
#pragma warning(disable:26812)
void WallFollowing::setAndDrawFollowingWay() {

	// Jump to the edge of previous and current bitmaps
	COORD prev_real = toRealCoord(prev);
	COORD curr_real = toRealCoord(curr);
	Console::gotoXY((prev_real.X + curr_real.X) / 2, (prev_real.Y + curr_real.Y) / 2);

	unsigned char colour = 0;



	// First, if it rotates, the corner will not be drawn or set. We have to set this thing
	// For rotating 180 degrees
	// since '<' = 60, '>' = 62, '^' = 94, 'v' = 118, we will not worry about the buggy sum
	if ((direction + prev_direction == '<' + '>') or (direction + prev_direction == '^' + 'v')) {
		BITMAP(prev).up_left = true;
		BITMAP(prev).up_right = true;
		BITMAP(prev).down_left = true;
		BITMAP(prev).down_right = true;
	}
	// For straight way (0 degrees) => just do nothing
	else if (direction == prev_direction) {

	}
	else {
		// If left following => the rotating in clockwise will be set at the corner, while in counterclockwise is not necessary
		// Apply to the right following in a different way
		if (left_follow) {
			if (direction == getRightSide(prev_direction)) {
				switch (prev_direction) {
				case '^': BITMAP(prev).up_left = true;		break;
				case '<': BITMAP(prev).down_left = true;	break;
				case 'v': BITMAP(prev).down_right = true;	break;
				case '>': BITMAP(prev).up_right = true;		break;
				}
			}
		}
		else {
			if (direction == getLeftSide(prev_direction)) {
				switch (prev_direction) {
				case '^': BITMAP(prev).up_right = true;		break;
				case '<': BITMAP(prev).up_left = true;		break;
				case 'v': BITMAP(prev).down_left = true;	break;
				case '>': BITMAP(prev).down_right = true;	break;
				}
			}
		}

	}
	


	switch (direction) {

	// Previous bitmap is on the left to current one
	case '>':
		// Set upward way if follow left wall
		if (left_follow) {
			BITMAP(prev).up_right = true;
			BITMAP(curr).up_left = true;
		}
		// Draw downard way if follow right wall
		else {
			BITMAP(prev).down_right = true;
			BITMAP(curr).down_left = true;
		}
		colour = (BITMAP(prev).up_right ? dark_yellow : white) * 16 + (BITMAP(prev).down_right ? dark_yellow : white);
		Console::setColour(colour);
		std::cout << std::string(2, BOTTOM_HALF_BLOCK);
		break;



	// Previous bitmap is on the right to current one
	case '<':
		if (left_follow) {
			BITMAP(prev).down_left = true;
			BITMAP(curr).down_right = true;
		}
		else {
			BITMAP(prev).up_left = true;
			BITMAP(curr).up_right = true;
		}
		colour = (BITMAP(prev).up_left ? dark_yellow : white) * 16 + (BITMAP(prev).down_left ? dark_yellow : white);
		Console::setColour(colour);
		std::cout << std::string(2, BOTTOM_HALF_BLOCK);
		break;



	// Previous bitmap is above current one
	case '^':
		// Draw upward way if follow left wall
		if (left_follow) {
			BITMAP(prev).up_left = true;
			BITMAP(curr).down_left = true;
		}
		// Draw downard way if follow right wall
		else {
			BITMAP(prev).up_right = true;
			BITMAP(curr).down_right = true;
		}

		colour = (BITMAP(prev).up_left ? dark_yellow : white) * 16;
		Console::setColour(colour);
		putchar(' ');

		colour = (BITMAP(prev).up_right ? dark_yellow : white) * 16;
		Console::setColour(colour);
		putchar(' ');

		break;



	// Previous bitmap is on the right to current one
	case 'v':
		if (left_follow) {
			BITMAP(prev).down_right = true;
			BITMAP(curr).up_right = true;
		}
		else {
			BITMAP(prev).down_left = true;
			BITMAP(curr).up_left = true;
		}

		colour = (BITMAP(prev).down_left ? dark_yellow : white) * 16;
		Console::setColour(colour);
		putchar(' ');

		colour = (BITMAP(prev).down_right ? dark_yellow : white) * 16;
		Console::setColour(colour);
		putchar(' ');

		break;
	}
	
	// Then previous and current position
	colour = 0;
	Console::gotoXY(prev_real);
	colour = (BITMAP(prev).up_left ? dark_yellow : white) * 16 + (BITMAP(prev).down_left ? dark_yellow : white);
	Console::setColour(colour);
	putchar(BOTTOM_HALF_BLOCK);
	colour = (BITMAP(prev).up_right ? dark_yellow : white) * 16 + (BITMAP(prev).down_right ? dark_yellow : white);
	Console::setColour(colour);
	putchar(BOTTOM_HALF_BLOCK);

	Sleep(36);

	colour = 0;
	Console::gotoXY(curr_real);
	colour = (BITMAP(curr).up_left ? dark_yellow : white) * 16 + (BITMAP(curr).down_left ? dark_yellow : white);
	Console::setColour(colour);
	putchar(BOTTOM_HALF_BLOCK);
	colour = (BITMAP(curr).up_right ? dark_yellow : white) * 16 + (BITMAP(curr).down_right ? dark_yellow : white);
	Console::setColour(colour);
	putchar(BOTTOM_HALF_BLOCK);

	Sleep(36);

	prev_direction = direction;
}


bool WallFollowing::haveWallAhead(Maze& maze) {
	switch (direction) {
	case '^': return (MAP(curr).up		? false : true); break;
	case '<': return (MAP(curr).left	? false : true); break;
	case 'v': return (MAP(curr).down	? false : true); break;
	case '>': return (MAP(curr).right	? false : true); break;
	default: return true; break;
	}
}


bool WallFollowing::haveWallOnLeft(Maze& maze) {
	switch (direction) {
	case '^': return (MAP(curr).left	? false : true); break;
	case '<': return (MAP(curr).down	? false : true); break;
	case 'v': return (MAP(curr).right	? false : true); break;
	case '>': return (MAP(curr).up		? false : true); break;
	default: return true; break;
	}
}


bool WallFollowing::haveWallOnRight(Maze& maze) {
	switch (direction) {
	case '^': return (MAP(curr).right	? false : true); break;
	case '<': return (MAP(curr).up		? false : true); break;
	case 'v': return (MAP(curr).left	? false : true); break;
	case '>': return (MAP(curr).down	? false : true); break;
	default: return true; break;
	}
}


void WallFollowing::moveAhead() {
	switch (direction) {
	case '^': curr.Y--; break;
	case '<': curr.X--; break;
	case 'v': curr.Y++; break;
	case '>': curr.X++; break;
	}
}


char WallFollowing::findPath(Maze& maze) {

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
	Unit* temp = &MAP(curr);
	do {
		direction = rand() % 4;
	} while (nearFreeUnit(temp->up, temp->left, temp->down, temp->right, direction));
	switch (direction) {
	case 0: direction = '^'; break;
	case 1: direction = '<'; break;
	case 2: direction = 'v'; break;
	case 3: direction = '>'; break;
	}

	Console::setColour(dark_yellow * 16);

	short pass_start_point_times = 1;
	short count = 16 * 16 * 3;
	while (count > 0) {

		prev = curr;

		// 1) It relies on a certain wall
		if (left_follow) {
			if (not haveWallOnLeft(maze)) {
				direction = getLeftSide(direction);
				moveAhead();
			}
			else if (not haveWallAhead(maze)) {
				moveAhead();
			}
			else {
				direction = getRightSide(direction);
			}
		}
		else {
			if (not haveWallOnRight(maze)) {
				direction = getRightSide(direction);
				moveAhead();
			}
			else if (not haveWallAhead(maze)) {
				moveAhead();
			}
			else {
				direction = getLeftSide(direction);
			}
		}

		setAndDrawFollowingWay();

		if (equalCoord(curr, maze.start_pos))
			pass_start_point_times++;
		count--;

		// For 4-direction crossroad, the maximum number will be four
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
