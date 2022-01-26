#include "mainmenu.h"


MainMenu::MainMenu() {
	TextColor(ColorCode_White * 16);
	fixWindowSize(80, 22);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
	_readTETRIS();
	_readTBlockFrame();
	_readCONSOLEAPP();

	menu_choice = 0;
	game_choice = 0;
	settings_choice = 0;
	temp = NULL;
	loadData();
}

MainMenu::~MainMenu() {

}

void setSpace(char& letter) {
	if (letter == '_')
		letter = ' ';
}

void MainMenu::loadData() {
	std::ifstream f;
	f.open("initial_data.txt");

	if (f.is_open()) {
		f >> highest_score_classic.score >> highest_score_construct.score >> highest_score_fortune.score;
		highest_score_classic.convertToString();
		highest_score_construct.convertToString();
		highest_score_fortune.convertToString();

		f >> sound >> music;

		f >> move_left >> move_right >> soft_drop >> hard_drop >> rotate_left >> rotate_right >> hold;
		setSpace(move_left); setSpace(move_right); setSpace(soft_drop); setSpace(hard_drop);
		setSpace(rotate_left); setSpace(rotate_right); setSpace(hold);
	}
	else {
		std::cout << "Cannot find the file named initial_data.txt";
	}
	f.close();
}

char getSpace(char& letter) {
	if (letter == ' ')
		return '_';
	return letter;
}

void MainMenu::saveData() {
	std::ofstream f;
	f.open("initial_data.txt");

	f << highest_score_classic.score << ' ' << highest_score_construct.score << ' ' << highest_score_fortune.score << '\n';
	f << sound << ' ' << music << ' ' << show_instant_block << '\n';
	f << getSpace(move_left) << ' ' << getSpace(move_right) << ' ' << getSpace(soft_drop) << ' ' << getSpace(hard_drop);
	f << ' ' << getSpace(rotate_left) << ' ' << getSpace(rotate_right) << ' ' << getSpace(hold);
	
	f.close();
}

void MainMenu::animateMainTitle() {
	_animateTETRIS();
	_animateTBlockFrame();
	_animateCONSOLEAPP();
}

void MainMenu::drawMainTitle() {
	TextColor(ColorCode_White * 16 + ColorCode_Blue);

	gotoXY(39, 14);
	putchar('V');

	gotoXY(39, 18);
	putchar('^');

	gotoXY(34, 16);
	switch (menu_choice) {
	case 0: std::cout << "Start Game!"; break;
	case 1: std::cout << "  Settings "; break;
	case 2: std::cout << "  About us "; break;
	case 3: std::cout << " Exit game "; break;
	}
}

void MainMenu::runMainTitle() {
	animateMainTitle();
	bool exit = false;
	char c;
	while (not exit) {
		drawMainTitle();

		c = toupper(_getch());
		switch (c) {

		case 'W': case 'A':
			menu_choice--;
			if (menu_choice < 0)
				menu_choice = 3;
			break;

		case 'S': case 'D':
			menu_choice++;
			if (menu_choice >= 4)
				menu_choice = 0;
			break;

		case 'Z': case ' ': case 13:		// Accept
			switch (menu_choice) {
			case 0: runNewGame();	break;
			case 1: settings();		break;
			case 2: aboutUs();		break;
			case 3: exit = true;	break;
			}
		}
	}
}

void MainMenu::clearChoice() {
	for (short i = 0; i < 7; i++) {
		gotoXY(31, 13 + i);
		std::cout << std::string(16, ' ');
	}
}

void MainMenu::drawNewGame() {
	gotoXY(37, 15);
	switch (game_choice) {
	case 0: std::cout << highest_score_classic.score_string;	break;
	case 1: std::cout << highest_score_construct.score_string;	break;
	case 2: std::cout << highest_score_fortune.score_string;	break;
	}

	gotoXY(32, 18);
	switch (game_choice) {
	case 0: std::cout << " Classic mode "; break;
	case 1: std::cout << "Construct mode"; break;
	case 2: std::cout << " Fortune mode "; break;
	}
}

void MainMenu::runNewGame() {
	clearChoice();
	TextColor(ColorCode_White * 16 + ColorCode_Blue);

	gotoXY(32, 13);
	std::cout << "- Best Score -";

	gotoXY(32, 16);
	std::cout << std::string(14, '_');

	gotoXY(39, 17);
	putchar('V');

	gotoXY(39, 19);
	putchar('^');

	gotoXY(54, 17);
	std::cout << "Press Esc to exit";

	char c;
	bool exit = false;
	while (not exit) {

		drawNewGame();
		c = toupper(_getch());

		switch (c) {
		case 'W': case 'A':
			game_choice--;
			if (game_choice < 0)
				game_choice = 2;
			break;

		case 'S': case 'D':
			game_choice++;
			if (game_choice >= 3)
				game_choice = 0;
			break;

		case 'Z': case ' ': case 13:		// Accept
			switch (game_choice) {
			case 0: newClassicGame();	exit = true;	break;
			case 1: newConstructGame();	exit = true;	break;
			case 2: newBadLuckGame();	exit = true;	break;
			}
		case 'X': case 27:	// Leave
			exit = true;
			break;
		}
	}
	gotoXY(54, 17);
	std::cout << std::string(17, ' ');
	clearChoice();
}

void MainMenu::newClassicGame() {

}

void MainMenu::newConstructGame() {

}

void MainMenu::newBadLuckGame() {

}

void printChar(char& letter) {
	if (letter == ' ')
		std::cout << "space";
	else
		putchar(letter);
}

void MainMenu::drawSettings() {
	gotoXY(46, 3);
	for (short i = 0; i < 13; i++) {
		if (settings_choice == i)
			TextColor(ColorCode_DarkBlue * 16 + ColorCode_White);
		else
			TextColor(ColorCode_White * 16 + ColorCode_Blue);

		switch (i) {
		case 0: gotoXY(46, 3); printChar(temp->move_left); break;
		case 1: gotoXY(46, 4); printChar(temp->move_right); break;
		case 2: gotoXY(46, 5); printChar(temp->rotate_left); break;
		case 3: gotoXY(46, 6); printChar(temp->rotate_right); break;
		case 4: gotoXY(46, 8); printChar(temp->soft_drop); break;
		case 5: gotoXY(46, 9); printChar(temp->hard_drop); break;
		case 6: gotoXY(46, 11); printChar(temp->hold); break;
		case 7: gotoXY(46, 13); putchar(temp->sound); break;
		case 8: gotoXY(46, 14); putchar(temp->music); break;
		case 9: gotoXY(46, 16); putchar(temp->show_instant_block); break;
		case 10: gotoXY(34, 18); std::cout << "Reset Score!"; break;
		case 11: gotoXY(28, 20); std::cout << "Cancel"; break;
		case 12: gotoXY(46, 20); std::cout << "OK"; break;
		}
	}
	TextColor(ColorCode_DarkBlue * 16 + ColorCode_White);
}

void MainMenu::catchLetter(char& letter) {
	gotoXY(18, 22);
	std::cout << "Press a keyboard to catch for playing a game";
	char c;
	do {
		c = toupper(_getch());
	} while (1);
	letter = c;
	gotoXY(18, 22);
	std::cout << std::string(45, ' ');
}

void MainMenu::settings() {
	clrscr();

	std::string get_line;
	std::ifstream f;
	f.open("main_menu/settings.txt");
	for (short i = 0; i < 20; i++) {
		getline(f, get_line);
		gotoXY(22, i + 2);
		std::cout << get_line;
	}

	temp = new Settings(sound, music, show_instant_block, move_left, move_right, soft_drop, hard_drop, rotate_left, rotate_right, hold);

	char c;
	bool exit = false;
	while (not exit) {

		drawSettings();
		c = toupper(_getch());

		switch (c) {
		case 'W': case 'A':
			game_choice--;
			if (game_choice < 0)
				game_choice = 2;
			break;

		case 'S': case 'D':
			game_choice++;
			if (game_choice >= 3)
				game_choice = 0;
			break;

		case 'Z': case ' ': case 13:		// Accept
			switch (game_choice) {
			case 0: catchLetter(temp->move_left); break;
			}
		case 'X': case 27:	// Leave
			exit = true;
			break;
		}
	}
	clrscr();
	_drawinstantMainMenu();

}

void MainMenu::aboutUs() {
	clrscr();

	std::string get_line;
	std::ifstream f;
	f.open("main_menu/thank_you.txt");
	for (short i = 8; i <= 16; i++) {
		getline(f, get_line);
		gotoXY(17, i);
		std::cout << get_line;
	}

	char c;
	bool exit = false;
	while (not exit) {

		c = toupper(_getch());

		switch (c) {
		case 'Q':
			ShellExecute(0, 0, L"http://www.google.com", 0, 0, SW_SHOW);
			// jump to default so no need to break

		default:
			exit = true;
			break;
		}
	}
	clrscr();
	_drawinstantMainMenu();
}
