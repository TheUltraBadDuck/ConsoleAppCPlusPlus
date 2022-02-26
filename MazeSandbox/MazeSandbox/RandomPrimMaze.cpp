#include "RandomPrimMaze.h"


void RandomPrimMaze::updateDirectionState() {
	SET_DIRECTION_VALUE(false);
	_up		= (curr.Y > 0) and (BITMAP_S(curr, 0, -1) == BitmapState::occupied);
	_left	= (curr.X > 0) and (BITMAP_S(curr, -1, 0) == BitmapState::occupied);
	_down	= (curr.Y < 15) and (BITMAP_S(curr, 0, 1) == BitmapState::occupied);
	_right	= (curr.X < 15) and (BITMAP_S(curr, 1, 0) == BitmapState::occupied);
}


void drawReadyBitmap(COORD& pos) {
	Console::gotoXY(toRealCoord(pos));
	std::cout << BLOCK;
}


#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
void RandomPrimMaze::setNeighbourList(COORD& pos) {

	BITMAP(pos) = BitmapState::occupied;

	COORD temp{};
	Console::setColour(cyan * 16);

	if ((pos.Y > 0) and (BITMAP_S(pos, 0, -1) == BitmapState::empty)) {
		temp = pos;
		temp.Y--;
		BITMAP(temp) = BitmapState::ready;
		pos_list.push_back(temp);
		drawReadyBitmap(temp);
	}

	if ((pos.X > 0) and (BITMAP_S(pos, -1, 0) == BitmapState::empty)) {
		temp = pos;
		temp.X--;
		BITMAP(temp) = BitmapState::ready;
		pos_list.push_back(temp);
		drawReadyBitmap(temp);
	}

	if ((pos.Y < 15) and (BITMAP_S(pos, 0, 1) == BitmapState::empty)) {
		temp = pos;
		temp.Y++;
		BITMAP(temp) = BitmapState::ready;
		pos_list.push_back(temp);
		drawReadyBitmap(temp);
	}

	if ((pos.X < 15) and (BITMAP_S(pos, 1, 0) == BitmapState::empty)) {
		temp = pos;
		temp.X++;
		BITMAP(temp) = BitmapState::ready;
		pos_list.push_back(temp);
		drawReadyBitmap(temp);
	}

	Console::setColour(white);
}


void RandomPrimMaze::generate(Maze& maze, COORD& pos) {

	// 0) First, let's choice a random position
	curr.X = rand() % 16;
	curr.Y = rand() % 16;

	setNeighbourList(curr);

	short count = 16 * 16 - 1;
	while (count > 0) {

		// 1) Choose a random element in the vector, then remove and choose it
		size_t id = rand() % pos_list.size();
		curr = pos_list[id];
		std::swap(pos_list[id], pos_list.back());
		pos_list.pop_back();

		// 2) Find the connection of the neighbours
		// 
		// NOTE: You will see this quite similar to Randomized DFS Algorithm.
		// The difference is this one will check with OCCUPIED UNIT, in order to connect it, instead of empty one
		updateDirectionState();

		// 3) Then random the valid direction then move
		// Choose the OCCUPIED neighbor
		char direction;
		do {
			direction = rand() % 4;
		} while (not nearFreeUnit(DIRECTION_GROUP, direction));

		setNeighbourList(curr);
		prev = curr;

		switch (direction) {
		case 0: curr.Y--; break;	// up
		case 1: curr.X--; break;	// left
		case 2:	curr.Y++; break;	// down
		case 3: curr.X++; break;	// right
		}

		// Update data
		Console::setColour(white * 16);
		maze.connect(prev, curr);
		maze.instantlyDrawUnit(prev);
		maze.instantlyDrawEdge(prev, curr);
		maze.instantlyDrawUnit(curr);

		count--;

		Sleep(30);
	}

	maze.cycling = false;
	maze.drawCursor(pos);
}
