#ifndef _SUPPORT_
#define _SUPPORT_


#include "console.h"


extern short square_frame[22][10];		// the frame of the whole game, the value is the colour, if negative: the block is moving
										// Height is actually 20. the extra 2 is for spawing
extern bool lose;


enum class BlockName {
	T_Block,
	L_Block,
	J_Block,
	I_Block,
	S_Block,
	Z_Block,
	O_Block,
	No_Data
};


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - -  TETROMINO  - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

const short LEFT_BORDER = 28;
const short UP_BORDER = 3;
const short WIDTH = 10;
const short HEIGHT = 20;
const short INIT_X = 4;
const short INIT_Y = -2;

// From Apparent to real
short apparentToRealX(short value);
short apparentToRealY(short value);
COORD apparentToRealCoord(COORD value);

// Each basic tetromino has four Units
struct Unit {
	COORD apparent{};
	COORD real{};
	Unit() {}
	Unit(COORD a) { apparent = a; real = apparentToRealCoord(a); }
	void setValue(COORD a) { apparent = a; real = apparentToRealCoord(a); }
};

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - -  BLOCKS - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


// Shift the coordinates of each virtual block
void _shiftVirtualBlock(std::vector<Unit>&, COORD);

// Rotate the sprite
void _rotateVirtualSprite(COORD sprite[], COORD duplicate[], short size, bool clockwise);

// When the new-born block is overlapping the square_frame, return true. You will lose here
bool _checkInitBlockOverlapping(std::vector<Unit>&);


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - GAME  - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


struct LargeNumber {
	std::string part[3];
	friend std::ostream& operator << (std::ostream& stream, LargeNumber& number);
}; static LargeNumber large_numbers[10];

void _readLargeNumber();
void _printLargeNumber(std::string string_line, COORD position);

struct WaitingBlock {
	std::string part[3];
}; static WaitingBlock waiting_blocks[7];

void _readBlock();
void _printBlock(BlockName type, COORD position, bool coloured);




struct Score {
	unsigned long score;
	std::string score_string;

	Score() { resetScore(); }

	std::string convertToString() {
		if (score > 99999) {
			score_string = "99999";
			return score_string;
		}
		std::string _score_string = std::to_string(score);
		for (unsigned char i = 0; i < _score_string.size(); i++)
			score_string.pop_back();
		score_string += _score_string;
		return score_string;
	}
	unsigned long operator+= (unsigned long value) {
		score += value;
		convertToString();
		return score;
	}
	void resetScore() { score = 0; score_string = "00000"; }
};


struct Level {
	unsigned long level;
	std::string level_string;

	Level() { resetScore(); }

	std::string convertToString() {
		std::string _level_string = std::to_string(level);
		for (unsigned char i = 0; i < _level_string.size(); i++) {
			level_string.pop_back();
		}
		level_string += _level_string;
		return level_string;
	}
	unsigned long operator+= (unsigned long value) {
		level += value;
		if (level > 999)
			level = 999;
		convertToString();
		return level;
	}
	void resetScore() { level = 1; level_string = "001"; }
};


bool _isLaneFull(short& lane);						// If the lane is full 10 units
void _shiftLane(short& lane, short& height);		// Move the needed lane to the one mentioned below
void _emptyLane(short& lane);						// Make the lane without block
void _printLane(short& lane);						// Output the lane (for test)
void _whitenLane(short& lane);						// Whiten the full lane

void _whitenBar(short& bar);						// Whiten the requiring bar


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - MAIN MENU - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


extern Score highest_score_classic, highest_score_construct, highest_score_fortune;
extern bool sound, music, show_instant_block;
extern char move_left, move_right, soft_drop, hard_drop, rotate_left, rotate_right, hold;



struct BigFont {
	std::string part[6];
	friend std::ostream& operator << (std::ostream& stream, BigFont& word);
}; static BigFont TETRIS[6];
static std::vector<std::string> t_block_frame;
static std::vector<std::string> CONSOLE_APP;

void _readTETRIS();
void _animateTETRIS();
void _readTBlockFrame();
void _animateTBlockFrame();
void _readCONSOLEAPP();
void _animateCONSOLEAPP();
void _drawinstantMainMenu();



#endif
