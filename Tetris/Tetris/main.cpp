#include "mainmenu.h"


short square_frame[22][10];
bool lose;

Score highest_score_classic, highest_score_construct, highest_score_fortune;
bool sound, music, show_instant_block;
char go_left, go_right, soft_drop, hard_drop, rotate_left, rotate_right, hold;


int main() {
	srand(static_cast<unsigned int>(time(NULL)));
	MainMenu main_menu;
	main_menu.runMainTitle();
	//Game game;
	//game.drawGame();
	//game.runGame();
}
