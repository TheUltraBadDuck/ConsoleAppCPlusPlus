#include "Drawing.h"


/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~    CONS / DEST    ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~


Drawing::Drawing() {

	// Maze Generator Icon
	std::ifstream f;
	f.open("images/MazeGeneratorIcon.txt");

	if (f.is_open()) {
		for (short i = 0; i < 7; i++) {
			for (short j = 0; j < 5; j++) {
				getline(f, generator_icons[i].images[j]);
			}
			getline(f, generator_icons[i].name);
			getline(f, generator_icons[i].info);
		}
	}
	else {
		std::cerr << "Error reading file images/MazeGeneratorIcon.txt\n";
	}
	f.close();

	// Maze Solver Icon
	f.open("images/MazeSolverIcon.txt");

	if (f.is_open()) {
		for (short i = 0; i < 7; i++) {
			for (short j = 0; j < 5; j++) {
				getline(f, solver_icons[i].images[j]);
			}
			getline(f, solver_icons[i].name);
			getline(f, solver_icons[i].info);
		}
	}
	else {
		std::cerr << "Error reading file images/MazeSolverIcon.txt\n";
	}
	f.close();

	// Arrow Icon
	f.open("images/ArrowIcon.txt");
	if (f.is_open()) {
		for (short i = 0; i < 3; i++) {
			arrow_icon[i].reserve(3);
			getline(f, arrow_icon[i]);
		}
	}
	else {
		std::cerr << "Error reading file images/ArrowIcon.txt\n";
	}
	f.close();

	// Button
	f.open("images/Button.txt");
	if (f.is_open()) {
		getline(f, buttons[0].images[0]);
		getline(f, buttons[0].images[1]);
		getline(f, buttons[0].images[2]);

		for (short i = 1; i < 9; i++) {
			buttons[i] = buttons[0];
			buttons[i].images[1][2] = i + '0';
		}

		buttons[0].images[1][2] = '0';
		buttons[6].images[1][2] = '8';
		buttons[7].images[1][2] = '9';
		buttons[8].images[1] = "|Esc|";

		for (short i = 0; i < 9; i++) {
			getline(f, buttons[i].name);
		}
	}
	else {
		std::cerr << "Error reading file images/Button.txt\n";
	}
	f.close();
}

/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~  Draw interface   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

void Drawing::drawInterface() {
	drawFile("images/MazeFrame.txt", COORD{ LEFT_BORDER - WIDTH, UP_BORDER - HEIGHT });
	drawFile("images/Title.txt", COORD{ 93, 6 });
	drawFile("images/ValidatingBox.txt", COORD{ 94, 24 });
}

void Drawing::drawSaveMenu() {

	drawFile("images/SaveLoadBox.txt", COORD{ 40, 12 });
	const std::string title = "SAVE YOUR MAZE";
	const std::string left_button = "[Number] save";
	const std::string right_button = "[X] exit";

	Console::gotoXY(53, 13);
	std::cout << title;
	Console::gotoXY(45, 18);
	std::cout << left_button;
	Console::gotoXY(67, 18);
	std::cout << right_button;
}

void Drawing::drawLoadMenu() {

	drawFile("images/SaveLoadBox.txt", COORD{ 40, 12 });
	const std::string title = "LOAD YOUR MAZE";
	const std::string left_button = "[Number] save";
	const std::string right_button = "[X] exit";

	Console::gotoXY(53, 13);
	std::cout << title;
	Console::gotoXY(45, 18);
	std::cout << left_button;
	Console::gotoXY(67, 18);
	std::cout << right_button;
}

/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~     Operate interface     ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

short Drawing::operateGeneratorChoice() {

	bool exit = false;
	short final_result = -1;

	drawArrowIcon(0, true);
	writeGeneratorInfo(generator_choice);

	while (not exit) {

		char c = toupper(_getch());

		switch (c) {

		case 'W': case 'A':
			generator_choice--;
			if (generator_choice < 0)
				generator_choice = 6;
			drawGeneratorIcon(generator_choice);
			clearInfo();
			writeGeneratorInfo(generator_choice);
			break;

		case 'S': case 'D':
			generator_choice++;
			if (generator_choice > 6)
				generator_choice = 0;
			drawGeneratorIcon(generator_choice);
			clearInfo();
			writeGeneratorInfo(generator_choice);
			break;

		case 27: case 'X': case '4':
			exit = true;
			clearInfo();
			drawArrowIcon(0, false);
			break;

		case 13: case ' ': case 'Z':
			final_result = generator_choice;
			exit = true;
			clearInfo();
			drawArrowIcon(0, false);
			break;
		}
	}
	return final_result;

}

short Drawing::operateSolverChoice() {

	bool exit = false;
	short final_result = -1;

	drawArrowIcon(1, true);
	writeSolverInfo(solver_choice);

	while (not exit) {

		char c = toupper(_getch());

		switch (c) {

		case 'W': case 'A':
			solver_choice--;
			if (solver_choice < 0)
				solver_choice = 6;
			drawSolverIcon(solver_choice);
			clearInfo();
			writeSolverInfo(solver_choice);
			break;

		case 'S': case 'D':
			solver_choice++;
			if (solver_choice > 6)
				solver_choice = 0;
			drawSolverIcon(solver_choice);
			clearInfo();
			writeSolverInfo(solver_choice);
			break;

		case 27: case 'X': case '5':
			exit = true;
			clearInfo();
			drawArrowIcon(1, false);
			break;

		case 13: case ' ': case 'Z':
			final_result = solver_choice;
			exit = true;
			clearInfo();
			drawArrowIcon(1, false);
			break;
		}
	}
	return final_result;

}

std::string* getFourMazeNames() {
	short number;
	std::string* maze_names = new std::string[4];

	std::ifstream f;
	f.open("saved_data/FourMazeNames.txt");

	if (f.is_open()) {
		for (short i = 0; i < 4; i++) {

			std::string* temp = &maze_names[i];

			f >> number;
			(*temp).reserve(14);
			getline(f, *temp);
			(*temp).erase((*temp).begin());

			if (*temp == "`")
				(*temp).clear();

			Console::gotoXY(42 + (i % 2) * 20, 15 + i / 2);
			std::cout << number << ". " << *temp;
		}
	}
	else {
		std::cerr << "Error reading file saved_data/FourMazeNames.txt\n";
	}
	f.close();

	return maze_names;
}

#pragma warning(push)
#pragma warning(disable: 4267)
std::string Drawing::operateSaveMenuChoice() {
	drawSaveMenu();

	std::string* maze_names = getFourMazeNames();
	std::string output_name;

	char id = 0;

	bool exit = false;
	bool exit2 = false;
	while (not exit) {

		char c = toupper(_getch());

		switch (c) {

		// If press the number
		case '1': case '2': case '3': case '4':

			id = c - '1';

			// If you want to replace the saved file
			if (maze_names[id].size() != 0) {
				std::string replace_warning = "Do you want to replace " + maze_names[id] + "? (Y)es or (N)o";
				Console::gotoXY((120 - replace_warning.size()) / 2, 20);
				std::cout << replace_warning;

				c = toupper(_getch());

				// If you REALLY want to replace => Clear the name first
				if (c == 'Y') {
					maze_names[id].clear();
				}
				// Else: just delete the last line and continue the loop
				else if (c == 'N') {
					Console::gotoXY((120 - replace_warning.size()) / 2, 20);
					std::cout << std::string(replace_warning.size(), ' ');
					break;	// This is 'break' in 'switch' but 'continue' in 'while'
				}
			}
			
			// Change the name by input
			Console::gotoXY(45 + (id % 2) * 20, 15 + id / 2);
			std::cout << std::string(12, ' ');
			Console::gotoXY(45 + (id % 2) * 20, 15 + id / 2);

			while (not exit2) {

				c = _getch();

				// Letter
				if (maze_names[id].size() < 12) {
					if (
						((c >= 'a') and (c <= 'z')) or (c >= 'A' and c <= 'Z') or
						(c == ' ') or (c == '_') or (c == '-') or (c == ',') or (c == '.') or
						(c == '(') or (c == ')')
						) {
						putchar(c);
						maze_names[id].push_back(c);
					}
				}

				// Backspace
				if ((c == 8) and (maze_names[id].size() > 0)) {
					std::cout << "\b \b";
					maze_names[id].pop_back();
				}

				// Enter
				if ((c == 13) and (maze_names[id].size() > 0)) {

					//Console::gotoXY(0, 10);
					//std::cout << maze_names[id].size() << " " << maze_names[id];
					//(void)_getch();

					output_name = maze_names[id];
					exit = true;
					exit2 = true;

					std::ofstream f;
					f.open("saved_data/FourMazeNames.txt");
					for (short i = 0; i < 4; i++) {
						f << i + 1 << ' ';
						if (maze_names[i].size() == 0)
							f << "`\n";
						else
							f << maze_names[i] << '\n';
					}
					f.close();

					break;
				}
			}
			break;

		case 13: case 'X':
			exit = true;
			break;
		}
	}

	delete[] maze_names;
	return output_name;
}

std::string Drawing::operateLoadMenuChoice() {
	drawLoadMenu();

	std::string* maze_names = getFourMazeNames();
	std::string output_name;

	char id = 0;

	bool exit = false;
	while (not exit) {
		
		char c = toupper(_getch());

		switch (c) {

		case '1': case '2': case '3': case '4':

			id = c - '1';
			// If there is a file => download and use it
			if (maze_names[id].size() > 0) {
				exit = true;
				output_name = maze_names[id];
			}
			break;

		case 13: case 'X':
			exit = true;
			break;
		}
	}

	return output_name;
}


/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~    Draw icon      ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

void Drawing::drawGeneratorIcon(short index) {

	short i;
	for (i = 0; i < 5; i++) {
		Console::gotoXY(generator_pos.X + 3, generator_pos.Y + i);
		std::cout << generator_icons[index].images[i];
	}
	Console::gotoXY(generator_pos.X, generator_pos.Y + i);
	std::cout << generator_icons[index].name;

}

void Drawing::drawSolverIcon(short index) {

	short i;
	for (i = 0; i < 5; i++) {
		Console::gotoXY(solver_pos.X + 3, solver_pos.Y + i);
		std::cout << solver_icons[index].images[i];
	}
	Console::gotoXY(solver_pos.X, solver_pos.Y + i);
	std::cout << solver_icons[index].name;
}

void Drawing::drawArrowIcon(short index, bool show) {

	COORD temp{};
	if (index == 0) {
		temp.X = generator_pos.X - 5;
		temp.Y = generator_pos.Y + 1;
	}
	else {
		temp.X = solver_pos.X - 5;
		temp.Y = solver_pos.Y + 1;
	}

	if (show) {
		for (short i = 0; i < 3; i++) {
			Console::gotoXY(temp);
			temp.Y++;
			std::cout << arrow_icon[i];
		}
	}
	else {
		for (short i = 0; i < 3; i++) {
			Console::gotoXY(temp);
			temp.Y++;
			std::cout << std::string(3, ' ');
		}
	}
}


void Drawing::drawButton(short index) {

	COORD pos{};

	// Set position of objects
	switch (index) {
	case 0: pos.X = 10; pos.Y = 38; break;
	case 1: pos.X = 30; pos.Y = 38; break;
	case 2: pos.X = 45; pos.Y = 38; break;
	case 3: pos.X = 45; pos.Y = 41; break;
	case 4: pos.X = 70; pos.Y = 38; break;
	case 5: pos.X = 70; pos.Y = 41; break;
	case 6: pos.X = 95; pos.Y = 38; break;
	case 7: pos.X = 95; pos.Y = 41; break;
	case 8: pos.X = 10; pos.Y = 41; break;
	}

	switch (index) {

	// For 'Draw', 'Generator', 'Compiler' , 'Save game', 'Load game' and 'Clear whole map' => Operate as holding the keyboard
	case 1: case 4: case 5: case 6: case 7: case 8:
		buttons[index].pressed = not buttons[index].pressed;
		buttons[index].draw(pos);
		break;

	// For 'Clear unit', 'Start point' and 'Finish point' => Operate as pressing the keyboard
	default:
		buttons[index].pressed = true;
		buttons[index].draw(pos);
		Sleep(30);
		buttons[index].pressed = false;
		buttons[index].draw(pos);

		break;
	}
}

void Drawing::drawAllButtons() {

	for (short i = 0; i < 9; i++) {

		COORD pos{};
		char id = 0;

		// Set position of objects
		switch (i) {
		case 0: pos.X = 10; pos.Y = 38; break;
		case 1: pos.X = 30; pos.Y = 38; break;
		case 2: pos.X = 45; pos.Y = 38; break;
		case 3: pos.X = 45; pos.Y = 41; break;
		case 4: pos.X = 70; pos.Y = 38; break;
		case 5: pos.X = 70; pos.Y = 41; break;
		case 6: pos.X = 95; pos.Y = 38; break;
		case 7: pos.X = 95; pos.Y = 41; break;
		case 8: pos.X = 10; pos.Y = 41; break;
		}

		buttons[i].draw(pos);
	}
}



/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~    Write info     ~   ~   ~   ~   ~   ~
/// ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~   ~

void Drawing::writeGeneratorInfo(short index) {
	Console::gotoXY((120 - generator_icons[index].info.size()) / 2, 37);
	std::cout << generator_icons[index].info;
}

void Drawing::writeSolverInfo(short index) {
	Console::gotoXY((120 - solver_icons[index].info.size()) / 2, 37);
	std::cout << solver_icons[index].info;
}

void Drawing::writeCustom(std::string line) {
	Console::gotoXY((120 - line.size()) / 2, 37);
	std::cout << line;
}

void Drawing::writeCompilingStatus(char status) {
	std::string compiling_info;

	switch (status) {

	case 0:
		compiling_info = "The path has been set. All are good. Press anything to continue.";
		Console::setColour(green);
		break;
	case 1:
		compiling_info = "Start point missing.";
		Console::setColour(red);
		break;
	case 2:
		compiling_info = "Finish point missing.";
		Console::setColour(red);
		break;
	case 3:
		compiling_info = "Start and finish point does not connect each other.";
		Console::setColour(red);
		break;
	case 4:
		compiling_info = "Cycle exists. This algorithm cannot solve this special maze.";
		Console::setColour(red);
		break;
	case 5:
		compiling_info = "Something else went wrong!";
		Console::setColour(red);
		break;

	}

	Console::gotoXY((120 - compiling_info.size()) / 2, 37);
	std::cout << compiling_info;
}

void Drawing::clearInfo() {
	Console::gotoXY(20, 37);
	std::cout << std::string(80, ' ');
}
