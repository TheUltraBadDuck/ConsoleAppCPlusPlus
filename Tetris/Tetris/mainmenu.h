#ifndef _MAIN_MENU_
#define _MAIN_MENU_


#include "game.h"


class MainMenu {

private:
//	Game game;
	short menu_choice;		// for main menu
	short game_choice;		// for new game
	short settings_choice;	// for settings
	Settings* temp;

public:
	MainMenu();
	~MainMenu();
	void loadData();
	void saveData();
	void animateMainTitle();
	void drawMainTitle();
	void runMainTitle();

	void clearChoice();
	
	// menu_choice = 0
	void drawNewGame();
	void runNewGame();
	void newClassicGame();		// Classic Tetris
	void newConstructGame();	// A game mode which already has blocks instead of empty frame
	void newBadLuckGame();		// A game mode which gives random buff

	// menu_choice = 1
	void drawSettings();
	void catchLetter(char& letter);
	void settings();

	// menu_choice = 2
	void aboutUs();


};


struct Settings {
	bool sound, music, show_instant_block;
	char move_left, move_right, soft_drop, hard_drop, rotate_left, rotate_right, hold;
	Settings(
		bool sound, bool music, bool show_instant_block,
		char move_left, char move_right, char soft_drop, char hard_drop, char rotate_left, char rotate_right, char hold
	) {
		setTempSettings(sound, music, show_instant_block, move_left, move_right, soft_drop, hard_drop, rotate_left, rotate_right, hold);
	}
	void setTempSettings(
		bool sound, bool music, bool show_instant_block,
		char move_left, char move_right, char soft_drop, char hard_drop, char rotate_left, char rotate_right, char hold
	) {
		this->sound = sound;
		this->music = music;
		this->show_instant_block = show_instant_block;
		this->move_left = move_left;
		this->move_right = move_right;
		this->soft_drop = soft_drop;
		this->hard_drop = hard_drop;
		this->rotate_left = rotate_left;
		this->rotate_right = rotate_right;
		this->hold = hold;
	}
};


#endif
