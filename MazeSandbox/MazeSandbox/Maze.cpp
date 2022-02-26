#include "Maze.h"


/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~    CONS / DEST    ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

Maze::Maze() {
	strcpy_s(BLOCK, 3, "  ");
	strcpy_s(CURSOR, 3, "()");
}

Maze::~Maze() {

}

/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~  Setter / Getter  ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

void Maze::setState(COORD& pos) {
	
	Unit* p_unit = &map[pos.Y][pos.X];

	// If the unit is Start Point => Start State
	if (equalCoord(pos, start_pos))
		p_unit->state = UnitState::start_point;

	// Ditto
	else if (equalCoord(pos, finish_pos))
		p_unit->state = UnitState::finish_point;

	// If the unit is in any connection in four directions => Block State
	else if (p_unit->up or p_unit->left or p_unit->down or p_unit->right)
		p_unit->state = UnitState::block;

	// Else
	else
		p_unit->state = UnitState::empty;

}

/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~      Connect      ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

void Maze::connect(COORD& prev, COORD& curr) {

	// if the control moves right
	if (prev.X < curr.X) {
		map[prev.Y][prev.X].right = true;
		map[curr.Y][curr.X].left = true;
	}
	// if moves left
	else if (curr.X < prev.X) {
		map[prev.Y][prev.X].left = true;
		map[curr.Y][curr.X].right = true;
	}
	// if moves up
	else if (prev.Y > curr.Y) {
		map[prev.Y][prev.X].up = true;
		map[curr.Y][curr.X].down = true;
	}
	// if moves down
	else {
		map[prev.Y][prev.X].down = true;
		map[curr.Y][curr.X].up = true;
	}

	// then remake the state
	setState(prev);
	setState(curr);
}

/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~      Drawing      ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

// This one draw from top-left point to the center one only
// Except for drawing 2x2 square cycle
void Maze::drawUnit(COORD& pos) {

	Console::setColour(white * 16);

	// Draw Vertices 2 x 2
	//    O   O
	//         
	//    O   CP (Center point)
	for (short i = 0; i < 4; i++) {

		COORD temp{ pos.X + i % 2 - 1, pos.Y + i / 2 - 1 };

		if ((temp.X < 0) or (temp.X > 15) or (temp.Y < 0) or (temp.Y > 15))
			continue;

		switch (map[temp.Y][temp.X].state) {

		case UnitState::block:
			instantlyDrawUnit(temp);
			break;

		case UnitState::start_point:
			Console::setColour(red * 16);
			instantlyDrawUnit(temp);
			Console::setColour(white * 16);
			break;

		case UnitState::finish_point:
			Console::setColour(green * 16);
			instantlyDrawUnit(temp);
			Console::setColour(white * 16);
			break;
		}
	}

	// Draw horizontal edge 2 x 1
	//    O - O
	//         
	//    O - CP
	for (short i = 0; i < 2; i++) {

		COORD left{ pos.X - 1, pos.Y + i % 2 - 1 };
		COORD right = left;
		right.X++;

		if ((left.X < 0) or (right.X > 15) or (left.Y < 0) or (left.Y > 15))
			continue;

		if (map[left.Y][left.X].right == true)
			instantlyDrawEdge(left, right);
	}

	// Draw horizontal edge 3 x 2
	//    O   O
	//    |   |
	//    O   CP
	for (short i = 0; i < 2; i++) {

		COORD up{ pos.X + i % 2 - 1, pos.Y - 1 };
		COORD down = up;
		down.Y++;

		if ((up.X < 0) or (up.X > 15) or (up.Y < 0) or (down.Y > 15))
			continue;

		if (map[up.Y][up.X].down == true)
			instantlyDrawEdge(up, down);
	}

	// Draw square (if it exists a 2x2 cycle)


	for (short i = 0; i < 4; i++) {

		COORD temp{ pos.X + i % 2 - 1, pos.Y + i / 2 - 1 };

		if ((temp.X < 0) or (temp.X >= 15) or (temp.Y < 0) or (temp.Y >= 15))
			continue;

		if (
			map[temp.Y][temp.X].right and map[temp.Y][temp.X].down and
			map[temp.Y + 1][temp.X].right and map[temp.Y][temp.X + 1].down
			)
			instantlyDrawSquare(temp);
	}

	Console::setColour(white);
}

void Maze::drawWholeMap() {

	// Clear whole map first
	for (short i = 0; i < 31; i++) {
		Console::gotoXY(LEFT_BORDER, UP_BORDER + i);
		std::cout << std::string(62, ' ');
	}

	COORD I{};

	// White road
	Console::setColour(white * 16);
	for (I.X = 0; I.X < 16; I.X++) {
		for (I.Y = 0; I.Y < 16; I.Y++) {
			drawUnit(I);
		}
	}

	// Start point
	if (start_pos.X != -1) {
		Console::setColour(red * 16);
		Console::gotoXY(toRealCoord(start_pos));
		std::cout << BLOCK;
	}

	// Finish point
	if (finish_pos.X != -1) {
		Console::setColour(green * 16);
		Console::gotoXY(toRealCoord(finish_pos));
		std::cout << BLOCK;
	}

	Console::setColour(white);
}

void drawInsideBox(bool valid) {
	if (valid) {
		Console::setColour(green);
		std::cout << "OK!";
	}
	else {
		Console::setColour(red);
		std::cout << "/!\\";
	}
}

void Maze::setValidatingBoxValue() {
	Console::gotoXY(111, 26);	drawInsideBox(start_pos.X != -1);
	Console::gotoXY(111, 28);	drawInsideBox(finish_pos.X != -1);
	Console::gotoXY(111, 30);	drawInsideBox(winnable);
	Console::gotoXY(111, 32);	drawInsideBox(not cycling);
}

void Maze::drawStartPoint(COORD& pos) {

	if (equalCoord(start_pos, pos))
		return;

	// If the start point existed, delete it first
	if (start_pos.X != -1) {

		Unit* p_unit = &map[start_pos.Y][start_pos.X];
		// If there is connection => switch to block state

		if (p_unit->up or p_unit->left or p_unit->down or p_unit->right) {
			p_unit->state = UnitState::block;
			Console::setColour(white * 16);
		}
		else {
			p_unit->state = UnitState::empty;
			Console::setColour(white);
		}
		Console::gotoXY(toRealCoord(start_pos));
		std::cout << BLOCK;
	}

	// If the current block is finish point => remove it
	if (map[pos.Y][pos.X].state == UnitState::finish_point) {
		finish_pos.X = -1;
		finish_pos.Y = -1;
	}

	// Then print it
	Console::setColour(red * 16);
	Console::gotoXY(toRealCoord(pos));
	std::cout << BLOCK;
	start_pos = pos;
	map[pos.Y][pos.X].state = UnitState::start_point;

	Console::setColour(white);
}

void Maze::drawFinishPoint(COORD& pos) {

	// Like above
	if (finish_pos.X != -1) {

		Unit* p_unit = &map[finish_pos.Y][finish_pos.X];

		if (p_unit->up or p_unit->left or p_unit->down or p_unit->right) {
			p_unit->state = UnitState::block;
			Console::setColour(white * 16);
		}
		else {
			p_unit->state = UnitState::empty;
			Console::setColour(white);
		}
		Console::gotoXY(toRealCoord(finish_pos));
		std::cout << BLOCK;
	}

	if (map[pos.Y][pos.X].state == UnitState::start_point) {
		start_pos.X = -1;
		start_pos.Y = -1;
	}

	// Then print it
	Console::setColour(green * 16);
	Console::gotoXY(toRealCoord(pos));
	std::cout << BLOCK;
	finish_pos = pos;
	map[pos.Y][pos.X].state = UnitState::finish_point;

	Console::setColour(white);
}

void Maze::redrawStartAndFinish() {
	Console::setColour(green * 16);
	instantlyDrawUnit(finish_pos);
	Console::setColour(red * 16);
	instantlyDrawUnit(start_pos);
	Console::setColour(white);
}

void Maze::instantlyDrawUnit(COORD& pos) {
	Console::gotoXY(toRealCoord(pos));
	std::cout << BLOCK;
}

void Maze::instantlyDrawEdge(COORD& prev, COORD& curr) {
	COORD temp = toRealCoord(prev);
	if (prev.Y > curr.Y)			// up
		temp.Y--;
	else if (prev.X > curr.X)		// left
		temp.X -= WIDTH;
	else if (prev.Y < curr.Y)		// down
		temp.Y++;
	else if (prev.X < curr.X)		// right
		temp.X += WIDTH;
	else
		return;

	Console::gotoXY(temp);
	std::cout << BLOCK;
}

void Maze::instantlyDrawSquare(COORD& up_left) {
	COORD temp = toRealCoord(up_left);
	temp.X += WIDTH;
	temp.Y++;
	Console::gotoXY(temp);
	std::cout << BLOCK;
}

/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~     Clearing      ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

void Maze::clearAroundUnit(COORD& pos) {

	Unit* p_unit = &map[pos.Y][pos.X];

	// Delete the connection
	if (p_unit->up) {
		p_unit->up = false;
		map[pos.Y - 1][pos.X].down = false;
	}

	if (p_unit->left) {
		p_unit->left = false;
		map[pos.Y][pos.X - 1].right = false;
	}

	if (p_unit->down) {
		p_unit->down = false;
		map[pos.Y + 1][pos.X].up = false;
	}

	if (p_unit->right) {
		p_unit->right = false;
		map[pos.Y][pos.X + 1].left = false;
	}

	p_unit->state = UnitState::empty;

	// If start point or finish point is deleted
	if (equalCoord(start_pos, pos)) {
		start_pos.X = -1;
		start_pos.Y = -1;
	}
	else if (equalCoord(finish_pos, pos)) {
		finish_pos.X = -1;
		finish_pos.Y = -1;
	}

	// Clear the drawing around the current unit

	// Clear the center point
	instantlyDrawUnit(pos);

	// Clear the edge at four direction and remove the point if there is no connection of them
	COORD dir_pos_list[4] = {
		{pos.X, pos.Y - 1},
		{pos.X - 1, pos.Y},
		{pos.X, pos.Y + 1},
		{pos.X + 1, pos.Y}
	};

	for (short i = 0; i < 4; i++) {
		COORD temp = dir_pos_list[i];

		if ((temp.X < 0) or (temp.X > 15) or (temp.Y < 0) or (temp.Y > 15))
			continue;

		instantlyDrawEdge(temp, pos);

		Unit* p_unit = &map[temp.Y][temp.X];
		if (not (p_unit->up or p_unit->left or p_unit->down or p_unit->right)) {
			instantlyDrawUnit(temp);
			p_unit->state = UnitState::empty;
		}
	}

	// Finally remove the square
	for (short i = 0; i < 4; i++) {
		COORD temp{ pos.X + i % 2 - 1, pos.Y + i / 2 - 1 };

		if ((temp.X < 0) or (temp.X >= 15) or (temp.Y < 0) or (temp.Y >= 15))
			continue;

		instantlyDrawSquare(temp);
	}

}

void Maze::clearWholeMap(bool effect) {

	if (effect) {
		
		COORD temp{};

		for (short step = 0; step < 65; step++) {

			// Draw the effect
			if (step < 61) {

				temp.X = step * WIDTH + LEFT_BORDER;
				temp.Y = UP_BORDER;

				Console::setColour(light_grey * 16);
				do {

					if ((temp.X < RIGHT_BORDER) and (temp.Y < DOWN_BORDER)) {
						Console::gotoXY(temp);
						std::cout << BLOCK;
					}

					temp.X -= WIDTH;
					temp.Y++;

				} while (temp.X >= LEFT_BORDER);
			}

			// Clear the effect
			if (step >= 4) {

				temp.X = step * WIDTH + LEFT_BORDER - WIDTH * 4;
				temp.Y = UP_BORDER;

				Console::setColour(white);
				do {
					if ((temp.X < RIGHT_BORDER) and (temp.Y < DOWN_BORDER)) {
						Console::gotoXY(temp);
						std::cout << BLOCK;
					}

					temp.X -= WIDTH;
					temp.Y++;

				} while (temp.X >= LEFT_BORDER);
			}

			Sleep(45);
		}
	}
	else {
		COORD temp = toRealCoord(COORD{ 0, 0 });
		for (short i = 0; i < 31; i++) {
			Console::gotoXY(temp);
			std::cout << std::string(62, ' ');
			temp.Y++;
		}
	}

	for (short i = 0; i < 16; i++) {
		for (short j = 0; j < 16; j++) {
			map[j][i].state = UnitState::empty;
			map[j][i].up = false;
			map[i][j].left = false;
			map[i][j].down = false;
			map[i][j].right = false;
		}
	}

	start_pos.X = -1;
	start_pos.Y = -1;
	finish_pos.X = -1;
	finish_pos.Y = -1;
	winnable = false;
	cycling = false;

}


void changeColourByState(UnitState state) {
	switch (state) {

	case UnitState::block:
		Console::setColour(white * 16);
		break;

	case UnitState::finish_point:
		Console::setColour(green * 16);
		break;

	case UnitState::start_point:
		Console::setColour(red * 16);
		break;

	default:
		Console::setColour(white);
		break;
	}
}

void Maze::drawCursor(COORD& prev, COORD& curr) {

	if ((prev.X == curr.X) and (prev.Y == curr.Y))
		return;

	Console::gotoXY(toRealCoord(prev));
	changeColourByState(map[prev.Y][prev.X].state);
	std::cout << BLOCK;

	Console::gotoXY(toRealCoord(curr));
	changeColourByState(map[curr.Y][curr.X].state);
	std::cout << CURSOR;

	Console::setColour(white);
}

void Maze::drawCursor(COORD& pos) {
	Console::gotoXY(toRealCoord(pos));
	changeColourByState(map[pos.Y][pos.X].state);
	std::cout << CURSOR;

	Console::setColour(white);
}

void Maze::deleteCursor(COORD& pos) {
	Console::gotoXY(toRealCoord(pos));
	changeColourByState(map[pos.Y][pos.X].state);
	std::cout << BLOCK;
}

/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~     Conversion    ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

COORD toRealCoord(COORD pos) {
	COORD temp{
		pos.X * WIDTH * 2 + LEFT_BORDER,
		pos.Y * HEIGHT * 2 + UP_BORDER
	};
	return temp;
}

COORD toApparentCoord(COORD pos) {
	COORD temp{
		(pos.X - LEFT_BORDER) / WIDTH / 2,
		(pos.Y - UP_BORDER) / HEIGHT / 2
	};
	return temp;
}



void Maze::saveGame() {

	if (maze_name.size() == 0)
		return;

	std::string file_name = "saved_data/" + maze_name + ".bin";

	std::ofstream f;
	f.open(file_name, std::ios::out | std::ios::binary);

	// Save into bit value: 8 bit as 12345678
	// bit 8 and 7: UnitState
	// bit 6: up
	// bit 5: left
	// bit 4: right
	// bit 3: down

	if (f) {

		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {

				Unit* p_unit = &map[i][j];

				char bit = 0;

				// State
				switch (p_unit->state) {

				case UnitState::empty:			bit |= 0;	break;
				case UnitState::block:			bit |= 1;	break;
				case UnitState::start_point:	bit |= 2;	break;
				case UnitState::finish_point:	bit |= 3;	break;
				}

				bit |= p_unit->up << 2;
				bit |= p_unit->left << 3;
				bit |= p_unit->down << 4;
				bit |= p_unit->right << 5;

				f.write((const char*)&bit, sizeof(char));
			}
		}
	}
	else {
		std::cerr << "Error opening file saved_data/ " << maze_name << ".bin";
		return;
	}

	f.close();

	if (not f.good()) {
		std::cerr << "Error occurred at writing time. File: saved_data/ " << maze_name << ".bin";
	}
}

void Maze::loadGame() {
	if (maze_name.size() == 0)
		return;

	std::string file_name = "saved_data/" + maze_name + ".bin";

	std::ifstream f;
	f.open(file_name, std::ios::in | std::ios::binary);

	if (f.is_open()) {

		start_pos.X = -1;
		start_pos.Y = -1;
		finish_pos.X = -1;
		finish_pos.Y = -1;

		for (short i = 0; i < 16; i++) {
			for (short j = 0; j < 16; j++) {

				Unit* p_unit = &map[i][j];

				char bit = 0;

				f.read((char*)&bit, sizeof(char));

				switch (bit & 3) {
				case 0:
					p_unit->state = UnitState::empty;
					break;
				case 1:
					p_unit->state = UnitState::block;
					break;
				case 2:
					p_unit->state = UnitState::start_point;
					start_pos.X = j;
					start_pos.Y = i;
					break;
				case 3:
					p_unit->state = UnitState::finish_point;
					finish_pos.X = j;
					finish_pos.Y = i;
					break;
				}

				p_unit->up = bit & (1 << 2);
				p_unit->left = bit & (1 << 3);
				p_unit->down = bit & (1 << 4);
				p_unit->right = bit & (1 << 5);
			}
		}
	}
	else {
		std::cerr << "Error opening file saved_data/ " << maze_name << ".bin";
		return;
	}

	f.close();
}