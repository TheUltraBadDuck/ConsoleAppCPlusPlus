#include "MazeInterface.h"


char BLOCK[3], CURSOR[3];


int main() {
	srand(static_cast<unsigned int>(time(NULL)));

	Console(COORD{120, 40}, true, false, false);
	MazeInterface project;
	project.runProject();
}
