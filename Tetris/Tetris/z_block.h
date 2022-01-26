#pragma once


#include "tetromino.h"


//
//    [][]
//      CP[]
//


class Z_Block : public Tetromino {

private:
	COORD sprite[4] = {
		{0, 0},
		{0, -1},
		{-1, -1},
		{1, 0}
	};

public:
	Z_Block(short);

	void checkRotate(bool clockwise);
	bool overlappingBlock(std::vector<Unit>& virtual_blocks);
	void rotateClockwise(std::vector<Unit>& virtual_blocks, COORD new_sprite[], COORD shift);
	void createBlock(std::vector<Unit>& units, COORD sprite[]);
};

