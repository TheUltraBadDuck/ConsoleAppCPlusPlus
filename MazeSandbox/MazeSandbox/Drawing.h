#ifndef _DRAWING_H
#define _DRAWING_H


#include "Console.h"


const short LEFT_BORDER = 28;
const short UP_BORDER = 5;

const short WIDTH = 2;
const short HEIGHT = 1;

const short RIGHT_BORDER = LEFT_BORDER + 31 * WIDTH;
const short DOWN_BORDER = UP_BORDER + 31 * HEIGHT;


extern char BLOCK[3];		// This block is for drawing (colour = white * 16) and deleting (colour = white)
extern char CURSOR[3];		// This block is for cursor


const COORD generator_pos{8, 6};
const COORD solver_pos{8, 15};


struct Icon {
	std::string images[5];
	std::string name;
	std::string info;

	Icon() {
		for (short i = 0; i < 5; i++) {
			images[i].reserve(9);
		}
		name.reserve(16);
	}
};


struct Button {
	std::string images[3];
	std::string name;
	bool pressed = false;

	Button() {
		images[0].reserve(5);
		images[1].reserve(5);
		images[2].reserve(5);
	}

	void draw(COORD pos) {
		if (pressed) {
			if (name == "Start point")
				Console::setColour(red);
			else if (name == "Finish point")
				Console::setColour(green);
			else
				Console::setColour(cyan);
		}
		else {
			Console::setColour(light_grey);
		}

		for (short i = 0; i < 3; i++) {
			Console::gotoXY(pos);
			std::cout << images[i];
			pos.Y++;
		}
		std::cout << ' ' << name;

		Console::setColour(white);
	}
};


class Drawing {

private:

	// Maze Generator
	short generator_choice = 0;
	Icon generator_icons[7];

	// Maze solver
	short solver_choice = 0;
	Icon solver_icons[7];

	// Arrow Icon
	short main_choice = 0;		// This one is the choice between 'generator choice' and 'compiler choice'
								// 0 for 'generator choice' and 1 for 'compiler choice'
	std::string arrow_icon[3];

	// Button
	Button buttons[9];			// 0: 0 - Clear				1: 1 - Draw				2: 2 - Start point
								// 3: 3 - Finish point		4: 4 - Generator		5: 5 - Compiler
								// 6: 8 - Save				7: 9 - Load				8: Esc - Clear Whole map


	short compiler_choice = 0;
	// Icon compiler_icons[8];
	
public:
	Drawing();

	// Draw interface
	void drawInterface();
	void drawSaveMenu();
	void drawLoadMenu();

	// Operate interface
	short operateGeneratorChoice();				// Return the type of Maze algorithm type
	short operateSolverChoice();
	std::string operateSaveMenuChoice();
	std::string operateLoadMenuChoice();

	// Draw icon
	void drawGeneratorIcon(short index);
	void drawSolverIcon(short index);
	void drawArrowIcon(short index, bool);
	void drawButton(short index);
	void drawAllButtons();

	// Write info
	void writeGeneratorInfo(short index);
	void writeSolverInfo(short index);
	void writeCustom(std::string line);
	void writeCompilingStatus(char status);		// 0: All are good		1: Start point missing		2: Finish point missing
												// 3: Unwinnable		4: Cycle existing			5: Something else
	void clearInfo();

};


#endif
