#pragma once


#include "tetromino.h"


//
//   CP[]
//   [][]
//


class O_Block : public Tetromino {


private:
	COORD sprite[4] = {
		{0, 0},
		{0, 1},
		{1, 1},
		{1, 0}
	};

public:
	O_Block(short);

	void checkRotate(bool clockwise) {}
	void createBlock(std::vector<Unit>& units, COORD sprite[]);
};

