#include "Console.h"


Console::Console(COORD window_size, bool show_cursor, bool resizable, bool selectable) {
    SetConsoleTitle(L"Maze Sandbox");

    fixWindowSize(window_size);
//    setScreenBufferSize(window_size);
    //showCursor(show_cursor);
    //if (resizable)
    //    disableResizeWindow();
    //if (selectable)
    //    disableSelection();

    SetConsoleOutputCP(CP_UTF8);
}

void Console::setColour(int colour) {
    HANDLE hOut;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, colour);
}

void Console::gotoXY(short x, short y) {
    COORD coord{x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void Console::gotoXY(COORD coord) {
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

COORD Console::getXY() {
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!GetConsoleScreenBufferInfo(hStd, &screenBufferInfo))
        printf("GetConsoleScreenBufferInfo (%d)\n", GetLastError());
    return screenBufferInfo.dwCursorPosition;
}

void Console::fixWindowSize(COORD window_size) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    
    SMALL_RECT WindowSize{};
    WindowSize.Top = 0;
    WindowSize.Left = 0;
    WindowSize.Right = window_size.X;
    WindowSize.Bottom = window_size.Y;

    SetConsoleWindowInfo(hStdout, 1, &WindowSize);
}

void Console::showCursor(bool show_cursor) {
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ConCurInf;

    ConCurInf.dwSize = 10;
    ConCurInf.bVisible = show_cursor;

    SetConsoleCursorInfo(handle, &ConCurInf);
}

void Console::setScreenBufferSize(COORD global_window_size) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(hStdout, global_window_size);
}

void Console::disableResizeWindow() {
    HWND hWnd = GetConsoleWindow();
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

void Console::disableSelection() {
    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(hStdin, ~ENABLE_QUICK_EDIT_MODE);
}



void drawFile(std::string filename) {
    std::string line;

    std::ifstream f;
    f.open(filename);

    COORD coord = Console::getXY();

    if (f.is_open()) {
        while (getline(f, line)) {
            Console::gotoXY(coord);
            std::cout << line;
            coord.Y++;
        }
    }
    else {
        std::cerr << "Error reading file " << filename << "\n";
    }

    f.close();
}

void drawFile(std::string filename, COORD coord) {
    Console::gotoXY(coord);
    drawFile(filename);
}

bool equalCoord(COORD A, COORD B) {
    if ((A.X == B.X) and (A.Y == B.Y))
        return true;
    return false;
}

bool isInStack(std::stack<short> store, short number) {
    while (store.size() != 0) {
        if (store.top() == number)
            return true;
        store.pop();
    }
    return false;
}

bool isInQueue(std::queue<short> store, short number) {
    while (store.size() != 0) {
        if (store.front() == number)
            return true;
        store.pop();
    }
    return false;
}

bool nearFreeUnit(bool& go_up, bool& go_left, bool& go_down, bool& go_right, char& direction) {
    switch (direction) {
    case 0:	return go_up;		break;
    case 1:	return go_left;		break;
    case 2:	return go_down;		break;
    case 3:	return go_right;	break;
    default: return false;		break;
    }
}

bool jumpingOutside(char& direction, COORD& pos) {
    switch (direction) {	// This one is for NOT connecting the border

    case 0: if (pos.Y == 0) return true; break;
    case 1: if (pos.X == 0) return true; break;
    case 2: if (pos.Y == 15) return true; break;
    case 3: if (pos.X == 15) return true; break;
    }
    return false;
}
