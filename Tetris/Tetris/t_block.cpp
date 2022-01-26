#include "t_block.h"


T_Block::T_Block(short colour) : Tetromino(colour) {
	type = BlockName::T_Block;
	createBlock(units, sprite);
	if (_checkInitBlockOverlapping(units))
		return;
	setGhostBlock();
	drawGhostBlock();
}



//
//    []              []
//  []CP[]    =>      CP[]
//                    []
//

void T_Block::checkRotate(bool clockwise) {

	// Rotate the sprite first
	COORD new_sprite[4]{};
	_rotateVirtualSprite(new_sprite, sprite, 4, clockwise);

	// Make a duplicate block that has rotated
	std::vector <Unit> virtual_units;
	createBlock(virtual_units, new_sprite);

	// Check if the rotation does not cause to touch any block
	// (look at check_virtual_block_position at tetromino.h)

	COORD shift{};

	for (short i = 0; i < 9; i++) {
		shift.X = check_virtual_block_position[i] / 3 - 1;
		shift.Y = check_virtual_block_position[i] % 3 - 1;

		_shiftVirtualBlock(virtual_units, shift);

		if (not overlappingBlock(virtual_units)) {
			rotateClockwise(virtual_units, new_sprite, shift);
			return;
		}

		_shiftVirtualBlock(virtual_units, COORD{ -shift.X, -shift.Y });
	}
}

void T_Block::createBlock(std::vector<Unit>& units, COORD sprite[]) {
	Unit new_unit;
	for (char i = 0; i < 4; i++) {
		new_unit.setValue(COORD{ sprite[i].X + center_apparent.X, sprite[i].Y + center_apparent.Y });
		units.push_back(new_unit);
	}
}

bool T_Block::overlappingBlock(std::vector<Unit>& virtual_units) {
	for (auto& unit : virtual_units) {
		if (unit.apparent.Y >= 20)
			return true;
		if ((unit.apparent.X < 0) or (unit.apparent.X >= 10))
			return true;
		if (square_frame[unit.apparent.Y + 2][unit.apparent.X] > 0)
			return true;
	}
	return false;
}

void T_Block::rotateClockwise(std::vector<Unit>& virtual_units, COORD new_sprite[], COORD shift) {
	removeGhostBlock();
	removeBlock();
	subtractSquareFrame();
	for (int i = 0; i < 4; i++) {
		sprite[i] = new_sprite[i];
		units[i] = virtual_units[i];
		units[i].setValue(units[i].apparent);
	}
	setGhostBlock();
	drawGhostBlock();
	drawBlock();
	addSquareFrame();
	virtual_units.clear();
}
