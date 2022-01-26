#include <stdio.h>
#include <conio.h>
#include "console.h"


short inputKey() {
	if (_kbhit()) {
		short key = _getch();

		if (key == 224)	{ // special key
			key = _getch();
			return key + 1000;
		}

		return key;
	}
	else {
		return key_none;
	}

	return key_none;
}

//-------------------------Screen-------------------------
void clrscr() {
	CONSOLE_SCREEN_BUFFER_INFO	csbiInfo;                  
	HANDLE	hConsoleOut;
	COORD	Home = {0,0};
	DWORD	dummy;

	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut,&csbiInfo);

	FillConsoleOutputCharacter(hConsoleOut,' ',csbiInfo.dwSize.X * csbiInfo.dwSize.Y,Home,&dummy);
	csbiInfo.dwCursorPosition.X = 0;
	csbiInfo.dwCursorPosition.Y = 0;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}


//screen: goto [x,y]
void gotoXY(short column, short line) {
	COORD coord{};
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


void gotoXY(COORD coordinates) {
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coordinates);
}


//screen: get [x]
unsigned short whereX() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return 10000;
}


//screen: get [y]
unsigned short whereY() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return 10000;
}


void TextColor (short color) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE) , color);
}


void fixWindowSize(short width, short height) {
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);

	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 800, 600, TRUE);
}

void drawFromFile(std::string file_name, COORD position, short colour) {
	std::string get_line;
	std::ifstream f;
	f.open(file_name);

	if (f.is_open()) {
		TextColor(colour);
		while (not f.eof()) {
			getline(f, get_line);
			gotoXY(position);
			std::cout << get_line;

			position.Y++;
		}
	}

	else {
		std::cerr << "Cannot find the file named" << file_name << "\n";
	}

	f.close();
}
