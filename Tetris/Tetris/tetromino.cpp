#include "tetromino.h"


Tetromino::Tetromino() {
	type = BlockName::No_Data;
	moving_time = 183;
	center_apparent = { INIT_X, INIT_Y };
	center_real = { apparentToRealX(INIT_X), apparentToRealY(INIT_Y) };
	colour = 15;
}

Tetromino::Tetromino(short colour) : Tetromino() {
	this->colour = colour;
}

Tetromino::~Tetromino() {

}



void Tetromino::moveLeft() {
	// Firstly check
	for (auto& unit : units) {
		if (
			(unit.apparent.X == 0) or			// If on the left border
			(square_frame[unit.apparent.Y + 2][unit.apparent.X - 1] > 0)		// or on the left there is a block
			)
			return;			// Then don't move
	}

	// Then remove
	removeGhostBlock();
	removeBlock();
	subtractSquareFrame();

	// Then change position
	center_apparent.X--;
	center_real.X -= 2;
	for (auto& unit : units) {
		unit.apparent.X--;
		unit.real.X -= 2;
	}

	// Finally draw
	setGhostBlock();
	drawGhostBlock();
	drawBlock();
	addSquareFrame();
}

void Tetromino::moveRight() {
	for (auto& unit : units) {
		if (
			(unit.apparent.X == WIDTH - 1) or
			(square_frame[unit.apparent.Y + 2][unit.apparent.X + 1] > 0)
			)
			return;
	}

	removeGhostBlock();
	removeBlock();
	subtractSquareFrame();

	center_apparent.X++;
	center_real.X += 2;
	for (auto& unit : units) {
		unit.apparent.X++;
		unit.real.X += 2;
	}

	setGhostBlock();
	drawGhostBlock();
	drawBlock();
	addSquareFrame();
}

void Tetromino::moveDown() {

	if (not isEmptyBelow())		// If the block touches something below
		return;

	TextColor(ColorCode_White);
	removeBlock();
	subtractSquareFrame();

	center_apparent.Y++;
	center_real.Y++;
	for (auto& unit : units) {
		unit.apparent.Y++;
		unit.real.Y++;
	}

	drawBlock();
	addSquareFrame();
}

void Tetromino::instantFall() {
	// Remove the whole real block
	removeBlock();
	subtractSquareFrame();

	units = instant_units;
	// Not need to change center coordinates
	moving_time = 0;

	drawBlock();
	addSquareFrame();
}



void Tetromino::addSquareFrame() {
	short i = 0;
	for (auto& unit : units) {
		square_frame[unit.apparent.Y + 2][unit.apparent.X] = -colour;	// see more at square_frame
	}
}

void Tetromino::subtractSquareFrame() {
	for (auto& unit : units) {
		square_frame[unit.apparent.Y + 2][unit.apparent.X] = 0;
	}
}

void Tetromino::stopBlock() {
	for (auto& unit : units) {
		square_frame[unit.apparent.Y + 2][unit.apparent.X] = colour;
	}
}



void Tetromino::drawBlock() {
	TextColor(colour * 17 - 128);
	for (auto& unit : units) {
		if (unit.apparent.Y >= 0) {
			gotoXY(unit.real);
			std::cout << "[]";
		}
	}
}

void Tetromino::removeBlock() {
	TextColor(ColorCode_White);
	for (auto& block : units) {
		if (block.apparent.Y >= 0) {
			gotoXY(block.real);
			std::cout << "  ";
		}
	}
}



void Tetromino::setGhostBlock() {
	instant_units.clear();
	instant_units = units;

	bool free_below = true;

	while (true) {
		// Check if there is free space to shift the block one
		for (auto& unit : instant_units) {
			if (unit.apparent.Y == 19) {
				free_below = false;
				break;
			}
			if (square_frame[unit.apparent.Y + 3][unit.apparent.X] != 0) {
				free_below = false;
				break;
			}
		}

		if (not free_below)
			break;

		// If valid then shift
		for (auto& unit : instant_units) {
			unit.apparent.Y++;
			unit.real.Y++;
		}
	}

}

void Tetromino::drawGhostBlock() {
	TextColor(ColorCode_Grey);
	for (auto& unit : instant_units) {
		if (unit.apparent.Y >= 0) {
			gotoXY(unit.real);
			std::cout << "[]";
		}
	}
}

void Tetromino::removeGhostBlock() {
	TextColor(ColorCode_White);
	for (auto& unit : instant_units) {
		if (unit.apparent.Y >= 0) {
			gotoXY(unit.real);
			std::cout << "  ";
		}
	}
}



bool Tetromino::isEmptyBelow() {
	for (auto& unit : units) {
		if (
			(unit.apparent.Y == HEIGHT - 1) or
			(square_frame[unit.apparent.Y + 3][unit.apparent.X] > 0)
			) {
			return false;
		}
	}
	return true;
}
