#include "o_block.h"


O_Block::O_Block(short colour) : Tetromino(colour) {
	type = BlockName::O_Block;
	createBlock(units, sprite);
	if (_checkInitBlockOverlapping(units))
		return;
	setGhostBlock();
	drawGhostBlock();
}


void O_Block::createBlock(std::vector<Unit>& units, COORD sprite[]) {
	Unit single_block;
	for (char i = 0; i < 4; i++) {
		single_block.setValue(COORD{ sprite[i].X + center_apparent.X, sprite[i].Y + center_apparent.Y });
		units.push_back(single_block);
	}
}
