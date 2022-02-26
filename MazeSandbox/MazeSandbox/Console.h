#ifndef _CONSOLE_H
#define _CONSOLE_H


#include <iostream>
#include <fstream>

//#include <iomanip>

#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <map>

#include <conio.h>
#include <stdlib.h>
//#include <time.h


#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#include <windows.h>


enum Colour {
    black, dark_blue, dark_green, dark_cyan,
    dark_red, dark_magneta, dark_yellow, light_grey,
    dark_grey, blue, green, cyan,
    red, magneta, yellow, white
};


class Console {

private:
    COORD window_size{};

public:
    Console(COORD window_size, bool show_cursor, bool resizable, bool selectable);

    static void setColour(int colour);
    static void gotoXY(short x, short y);
    static void gotoXY(COORD coord);
    static COORD getXY();
    
    static void fixWindowSize(COORD window_size);
    static void setScreenBufferSize(COORD global_window_size);
    static void disableResizeWindow();

    static void showCursor(bool show_cursor);
    static void disableSelection();
};


void drawFile(std::string filename);
void drawFile(std::string filename, COORD coord);


bool equalCoord(COORD A, COORD B);
bool isInStack(std::stack<short> store, short number);
bool isInQueue(std::queue<short> store, short number);


bool nearFreeUnit(bool&, bool&, bool&, bool&, char&);
bool jumpingOutside(char&, COORD&);

#endif
