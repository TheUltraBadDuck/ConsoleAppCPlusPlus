#ifndef _BLOCK_
#define _BLOCK_


// Also called blocks


#include "support.h"


class Tetromino {

protected:
	BlockName type;						// Name of the block

	// Note this is the coordinates of THE CENTER POINT OF TETROMINO
	COORD center_apparent{};			// apparent coordinates for frame, choose center
	COORD center_real{};				// real coordinates for drawing
	std::vector<Unit> units;			// The tetromino for the game
	std::vector<Unit> instant_units;	// For instantly placing when pressing 'Z' (see instantFall() and drawInstantBlock())
	
public:
	short keeping_lane;					// When staying, this value is equal to center_apparent.Y.
										// If this value is reset, the moving_time is as well
	short moving_time;					// When staying, this value will count down to 0, then completely stop
	short colour;

public:
	Tetromino();
	Tetromino(short);
	~Tetromino();

	// setter
	void setApparentX(short value) { center_apparent.X = value; center_real.X = apparentToRealX(value); }
	void setApparentY(short value) { center_apparent.Y = value; center_real.Y = apparentToRealY(value); }

	// getter
	BlockName getType() { return type; }
	short getApparentX() { return center_apparent.X; }
	short getApparentY() { return center_apparent.Y; }
	short getRealX() { return center_real.X; }
	short getRealY() { return center_real.Y; }
	short getMovingTime() { return moving_time; }
	short getColour() { return colour; }
	std::vector<Unit> getBlock() { return units; }

	// move
	void moveLeft();			// A
	void moveRight();			// D
	void moveDown();			// S
	virtual void checkRotate(bool clockwise) = 0;	// Q, E and W
	void instantFall();			// ' '

	// update square_frame
	void addSquareFrame();
	void subtractSquareFrame();
	void stopBlock();

	// draw real block
	void drawBlock();
	void removeBlock();

	// draw ghost block
	void setGhostBlock();
	void drawGhostBlock();
	void removeGhostBlock();

	// others
	bool isEmptyBelow();
	void subtractMovingTime() { moving_time--; }

	virtual void createBlock(std::vector<Unit>& units, COORD sprite[]) = 0;

};

//	If the block rotates, we have to make a virtual one for checking
//	[0] is the center point (CP). from [1] to [8] is the position to check
//
//		Coordinates for checking order:			to			Real coordinates
//
//															O--------------> x
//					[7] [6] [8]				--------\		|	0  1  2
//					[1] [0] [2]				--------/		|	3  4  5
//					[4] [3] [5]								|	6  7  8
//														y	V
//
//	Thus: [0] = 4, [1] = 3, etc.
//
//	The shift coordinates (figure 2 to figure 1): (x, y) = ([i] / 3 - 1, [i] % 3 - 1)
//

const char check_virtual_block_position[9] = { 4, 3, 5, 7, 6, 8, 1, 0, 2 };


#endif
