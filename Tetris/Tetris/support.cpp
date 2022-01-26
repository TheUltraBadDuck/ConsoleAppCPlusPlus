#include "support.h"


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - -  TETROMINO  - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


short apparentToRealX(short value) { return LEFT_BORDER + value * 2; }
short apparentToRealY(short value) { return UP_BORDER + value; }
COORD apparentToRealCoord(COORD value) { return { LEFT_BORDER + value.X * 2, UP_BORDER + value.Y }; }


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - -  BLOCKS - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


void _shiftVirtualBlock(std::vector<Unit>& virtual_units, COORD shift) {
	for (auto& units : virtual_units) {
		units.apparent.X += shift.X;
		units.apparent.Y += shift.Y;
	}
}

// Rotate Clockwise
//
//                | 0  1 |
// Matrix [x y] x | -1 0 | = [-y x]
//
void _rotateVirtualSprite(COORD sprite[], COORD duplicate[], short size, bool clockwise) {
	if (clockwise) {
		for (char i = 0; i < 4; i++) {
			sprite[i].X = -duplicate[i].Y;
			sprite[i].Y = duplicate[i].X;
		}
	}
	else {
		for (char i = 0; i < 4; i++) {
			sprite[i].X = duplicate[i].Y;
			sprite[i].Y = -duplicate[i].X;
		}
	}
}

bool _checkInitBlockOverlapping(std::vector<Unit>& units) {
	for (auto& unit : units) {
		if (square_frame[unit.apparent.Y + 2][unit.apparent.X] != 0) {
			lose = true;
			break;
		}
	}
	return lose;
}


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - GAME  - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


std::ostream& operator << (std::ostream& stream, LargeNumber& number) {
	short get_x = whereX(), get_y = whereY();
	stream << number.part[0];

	gotoXY(get_x, get_y + 1);
	stream << number.part[1];

	gotoXY(get_x, get_y + 2);
	stream << number.part[2];

	gotoXY(get_x + 3, get_y);
	return stream;
}

void _readLargeNumber() {
	std::ifstream f;
	f.open("drawing/number.txt");
	if (f.is_open()) for (short i = 0; i < 30; i++) getline(f, large_numbers[i / 3].part[i % 3]);
	else std::cout << "Cannot find the file named drawing/number.txt";
	f.close();
}

void _printLargeNumber(std::string string_line, COORD position) {
	TextColor(ColorCode_White); gotoXY(position);
	for (auto& i : string_line) std::cout << large_numbers[i - '0'];
}



void _readBlock() {
	std::ifstream f;
	f.open("drawing/block.txt");
	if (f.is_open()) for (short i = 0; i < 21; i++) getline(f, waiting_blocks[i / 3].part[i % 3]);
	else std::cout << "Cannot find the file named drawing/number.txt";
	f.close();
}

void _printBlock(BlockName type, COORD position, bool coloured) {
	if (type == BlockName::No_Data)
		return;
	short colour = 0;
	if (coloured) {
		switch (type) {
		case BlockName::T_Block: colour = ColorCode_Pink; break;
		case BlockName::L_Block: colour = ColorCode_White; break;
		case BlockName::J_Block: colour = ColorCode_Blue; break;
		case BlockName::I_Block: colour = ColorCode_Cyan; break;
		case BlockName::S_Block: colour = ColorCode_Green; break;
		case BlockName::Z_Block: colour = ColorCode_Red; break;
		case BlockName::O_Block: colour = ColorCode_Yellow; break;
		}
		colour = colour * 17 - 128;
	}
	else {
		colour = ColorCode_White;
	}
	WaitingBlock get_block = waiting_blocks[static_cast<short>(type)];
	for (char i = 0; i < 3; i++) {
		gotoXY(position);
		for (auto& j : get_block.part[i]) {
			if (j != ' ')
				TextColor(colour);
			else
				TextColor(ColorCode_White);
			std::cout << j;
		}
		position.Y++;
	}
}



// Remember to add 2!!!
// Remember to add 2!!!
// Remember to add 2!!! (see more at square_frame)
bool _isLaneFull(short& lane) {
	for (short j = 0; j < 10; j++) {
		if (square_frame[lane + 2][j] == 0)
			return false;
	}
	return true;
}

void _shiftLane(short& lane, short& height) {
	for (short j = 0; j < 10; j++) {
		square_frame[lane + 2][j] = square_frame[lane - height + 2][j];
	}
}

void _emptyLane(short& lane) {
	for (short j = 0; j < 10; j++) {
		square_frame[lane + 2][j] = 0;
	}
}

void _printLane(short& lane) {
	for (short j = 0; j < 10; j++) {
		std::cout << square_frame[lane + 2][j] << " ";
	}
}

void _whitenLane(short& lane) {
	TextColor(ColorCode_White * 16 + ColorCode_White);
	gotoXY(LEFT_BORDER, lane + UP_BORDER);
	std::cout << std::string(20, ' ');
}

void _whitenBar(short& bar) {
	short i = 0;
	TextColor(ColorCode_White * 16);		// White-coloured space
	if (bar < 12) {
		for (i; i < bar; i++) {
			gotoXY(LEFT_BORDER + 23, UP_BORDER + 16 - i);
			std::cout << "  ";
		}
	}
	else {
		for (i; i < 12; i++) {
			gotoXY(LEFT_BORDER + 23, UP_BORDER + 16 - i);
			std::cout << "  ";
		}
	}
}


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - MAIN MENU - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


std::ostream& operator << (std::ostream& stream, BigFont& word) {
	unsigned short get_x = whereX(), get_y = whereY();
	for (char i = 0; i < 6; i++) {
		stream << word.part[i];
		gotoXY(get_x, get_y + i + 1);
	}
	gotoXY(get_x + word.part[0].size() - 1, get_y);
	return stream;
}

void _readTETRIS() {
	std::ifstream f;
	f.open("main_menu/main_menu_title.txt");
	if (f.is_open()) for (short i = 0; i < 36; i++) getline(f, TETRIS[i / 6].part[i % 6]);
	else std::cout << "Cannot find the file named main_menu/main_menu_title.txt";
	f.close();
}

void _animateTETRIS() {
	const short colours[6] = { ColorCode_DarkRed, ColorCode_DarkYellow, ColorCode_Grey, ColorCode_DarkGreen, ColorCode_DarkCyan, ColorCode_DarkBlue };
	
	gotoXY(18, 6);
	for (short i = 0; i < 6; i++) {
		TextColor(ColorCode_White * 16 + ColorCode_DarkWhite);
		std::cout << TETRIS[i];
		gotoXY(whereX() - TETRIS[i].part[0].size() + 1, 6);

		Sleep(60);

		TextColor(colours[i] + ColorCode_White * 16);
		std::cout << TETRIS[i];
		gotoXY(whereX(), 6);
	}
	Sleep(60);
}

void _readTBlockFrame() {
	std::string get_line;

	std::ifstream f;
	f.open("main_menu/main_menu_title_2.txt");

	if (f.is_open()) {
		while (not f.eof()) {
			getline(f, get_line);
			t_block_frame.push_back(get_line);
		}
	}
	else {
		std::cout << "Cannot find the file named main_menu/main_menu_title_2.txt";
	}
	f.close();
}

// Acutally, just a drawing!
void _animateTBlockFrame() {
	Sleep(300);	// For reading files

	TextColor(ColorCode_White * 16 + ColorCode_Blue);
	for (unsigned short i = 0; i < t_block_frame.size(); i++) {
		gotoXY(13, 4 + i);
		for (unsigned short j = 0; j < t_block_frame[i].size(); j++) {
			if (t_block_frame[i][j] == ' ')
				gotoXY(whereX() + 1, whereY());
			else
				putchar(t_block_frame[i][j]);
		}
	}
	TextColor(ColorCode_White * 16);
}

void _readCONSOLEAPP() {
	std::string get_line;

	std::ifstream f;
	f.open("main_menu/main_menu_title_3.txt");

	if (f.is_open()) {
		while (not f.eof()) {
			getline(f, get_line);
			CONSOLE_APP.push_back(get_line);
		}
	}
	else {
		std::cout << "Cannot find the file named main_menu/main_menu_title_3.txt";
	}
	f.close();
}

void _animateCONSOLEAPP() {
	Sleep(100);	// For reading files

	TextColor(ColorCode_White * 16);
	for (short i = -3; i < 2; i++) {
		if (i >= 1) {
			gotoXY(8, i - 1);
			std::cout << std::string(64, ' ');
		}
		for (unsigned short j = 0; j < CONSOLE_APP.size(); j++) {
			if (i + j >= 0) {
				gotoXY(8, i + j);
				std::cout << CONSOLE_APP[j];
			}
			Sleep(50);
		}
	}
}

void _drawinstantMainMenu() {
	TextColor(ColorCode_White * 16 + ColorCode_Blue);
	for (unsigned short i = 0; i < t_block_frame.size(); i++) {
		gotoXY(13, 4 + i);
		std::cout << t_block_frame[i];
	}

	const short colours[6] = { ColorCode_DarkRed, ColorCode_DarkYellow, ColorCode_Grey, ColorCode_DarkGreen, ColorCode_DarkCyan, ColorCode_DarkBlue };
	gotoXY(18, 6);
	for (short i = 0; i < 6; i++) {
		TextColor(colours[i] + ColorCode_White * 16);
		std::cout << TETRIS[i];
		gotoXY(whereX(), 6);
	}

	TextColor(ColorCode_White * 16);
	for (unsigned short i = 0; i < CONSOLE_APP.size(); i++) {
		gotoXY(8, i + 2);
		std::cout << CONSOLE_APP[i];
	}
}
