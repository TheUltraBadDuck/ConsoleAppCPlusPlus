#include "game.h"


/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - CONSTRUCTOR AND DESTRUCTOR  - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

Game::Game() {
	delay = 100;
	requiring_bar = 0;
	running_delay = delay;

	holdable = true;
	held_block = BlockName::No_Data;

	for (short i = 0; i < 22; i++) {
		for (short j = 0; j < 10; j++) {
			square_frame[i][j] = 0;
		}
	}

	exit_game = false;

	_readLargeNumber();
	_readBlock();

	setWaitingBlocks();
}

Game::~Game() {

}

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - SETTER and GETTER - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void Game::setBlock(Tetromino*& new_tetromino) {
	if (new_tetromino != NULL)
		delete new_tetromino;

	BlockName block_temp = waiting_blocks.front();
	waiting_blocks.pop();

	switch (block_temp) {

	case BlockName::T_Block: new_tetromino = new T_Block(ColorCode_Pink); break;
	case BlockName::L_Block: new_tetromino = new L_Block(ColorCode_White); break;
	case BlockName::J_Block: new_tetromino = new J_Block(ColorCode_Blue); break;
	case BlockName::I_Block: new_tetromino = new I_Block(ColorCode_Cyan); break;
	case BlockName::S_Block: new_tetromino = new S_Block(ColorCode_Green); break;
	case BlockName::Z_Block: new_tetromino = new Z_Block(ColorCode_Red); break;
	case BlockName::O_Block: new_tetromino = new O_Block(ColorCode_Yellow); break;

	default: new_tetromino = NULL; break;
	}
}


BlockName Game::getRandomType() {
	char random = rand() % 7;
	switch (random) {

	case 0: return BlockName::T_Block; break;
	case 1: return BlockName::L_Block; break;
	case 2: return BlockName::J_Block; break;
	case 3: return BlockName::I_Block; break;
	case 4: return BlockName::S_Block; break;
	case 5: return BlockName::Z_Block; break;
	case 6: return BlockName::O_Block; break;

	default: return BlockName::No_Data; break;
	}
}


bool isThreeBlocksTheSame(std::queue<BlockName> blocks) {
	BlockName temp1 = blocks.front();
	blocks.pop();
	BlockName temp2 = blocks.front();
	blocks.pop();
	return (temp1 == temp2) and (temp1 == blocks.front());
}

void Game::setWaitingBlocks() {
PUSH_BACK:
	while (waiting_blocks.size() < 3) {
		waiting_blocks.push(getRandomType());
	}
	// If three waiting blocks are the same
	if (isThreeBlocksTheSame(waiting_blocks)) {
		waiting_blocks.pop();
		goto PUSH_BACK;
	}
}


void Game::setRequiringBar() {
	while (requiring_bar >= 12) {
		requiring_bar -= 12;
		level += 1;
		if (delay > 10)
			delay -= 10;
	}
}


void Game::setHeldBlock(Tetromino*& new_tetromino) {
	if (not holdable)
		return;

	new_tetromino->removeBlock();
	new_tetromino->subtractSquareFrame();
	new_tetromino->removeGhostBlock();

	// If the hold frame does not have a block
	if (held_block == BlockName::No_Data) {
		held_block = new_tetromino->getType();
		resetBlock(new_tetromino);
	}
	// Else, swap them
	else {
		BlockName block_temp = new_tetromino->getType();
		delete new_tetromino;
		switch (held_block) {

		case BlockName::T_Block: new_tetromino = new T_Block(ColorCode_Pink); break;
		case BlockName::L_Block: new_tetromino = new L_Block(ColorCode_White); break;
		case BlockName::J_Block: new_tetromino = new J_Block(ColorCode_Blue); break;
		case BlockName::I_Block: new_tetromino = new I_Block(ColorCode_Cyan); break;
		case BlockName::S_Block: new_tetromino = new S_Block(ColorCode_Green); break;
		case BlockName::Z_Block: new_tetromino = new Z_Block(ColorCode_Red); break;
		case BlockName::O_Block: new_tetromino = new O_Block(ColorCode_Yellow); break;

		default: new_tetromino = NULL; break;
		}
		held_block = block_temp;

		holdable = false;
	}

	drawHeldBlock();
}

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - -  DRAW GAME  - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

void Game::drawGame() {
	drawFromFile(
		"drawing/game_interface.txt",
		COORD{ 0, 4 },
		ColorCode_DarkWhite
	);
	drawFromFile(
		"drawing/game_interface2.txt",
		COORD{ 27, 2 },
		ColorCode_White
	);
	drawLevel();
	drawScore();
}


void Game::drawLevel() {
	_printLargeNumber(level.level_string, COORD{ 8, 5 });
}


void Game::drawScore() {
	_printLargeNumber(score.score_string, COORD{ 5, 11 });
}


void Game::drawTime() {

}


void Game::redrawGame() {
	for (short i = 2; i < 22; i++) {
		for (short j = 0; j < 10; j++) {
			gotoXY(apparentToRealX(j), apparentToRealY(i - 2));
			switch (square_frame[i][j]) {
				
			case 0:
				TextColor(ColorCode_White);
				std::cout << "  ";
				break;

			default:
				TextColor(square_frame[i][j] * 17 - 128);
				std::cout << "[]";
				break;
			}
		}
	}
}


void Game::drawHeldBlock() {
	_printBlock(held_block, COORD{ LEFT_BORDER + 36, UP_BORDER + 3 }, holdable);
}


void _drawWaitingBlocks(std::queue <BlockName> waiting_blocks) {
	short i = 0;
	while (not waiting_blocks.empty()) {
		_printBlock(waiting_blocks.front(), COORD{ LEFT_BORDER + 36, UP_BORDER + 8 + i++ * 4 }, true);
		waiting_blocks.pop();
	}
}

void Game::drawWaitingBlocks() {
	_drawWaitingBlocks(waiting_blocks);
}


void Game::drawRequiringBar() {
	short i;
	TextColor(ColorCode_Green * 16);		// Green-coloured space
	for (i = 0; i < requiring_bar; i++) {
		gotoXY(LEFT_BORDER + 23, UP_BORDER + 16 - i);
		std::cout << "  ";
	}

	TextColor(ColorCode_White);				// Black-coloured space (ColorCode_Black = 0)
	for (i; i < 12; i++) {
		gotoXY(LEFT_BORDER + 23, UP_BORDER + 16 - i);
		std::cout << "  ";
	}
}

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - RUN GAME  - - - - - - - - - - - - - 
/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 


void Game::runGame() {

	char c;
	Tetromino* new_tetromino = NULL;
	resetBlock(new_tetromino);

	while (not exit_game) {

		// If there is no valid block, add them
		if (new_tetromino->getMovingTime() == 0) {
			new_tetromino->instantFall();		// Avoid floating block if moving_time = 0
			new_tetromino->stopBlock();
			holdable = true;
			checkFullLane(new_tetromino);
			resetBlock(new_tetromino);
			drawHeldBlock();
		}

		if (lose) {
			if (new_tetromino != NULL)
				delete new_tetromino;
			exit_game = true;
			continue;
		}

		// Run the time
		Sleep(10);
		if (running_delay-- == 0) {
			running_delay = delay;
			new_tetromino->moveDown();
			//for (short i = -2; i < 20; i++) {
			//	gotoXY(0, i + 2);
			//	std::cout << i << ") ";
			//	_printLane(i);
			//}
		}

		if (not new_tetromino->isEmptyBelow())
			new_tetromino->subtractMovingTime();

		// Set the input
		if (_kbhit()) {
			c = toupper(_getch());

			switch (c) {

			case 'A':
				new_tetromino->moveLeft();
				break;

			case 'D':
				new_tetromino->moveRight();
				break;

			case 'Q': case 'W':
				new_tetromino->checkRotate(true);
				break;

			case 'E':
				new_tetromino->checkRotate(false);
				break;

			case 'S':
				new_tetromino->moveDown();
				break;

			case ' ': 
				new_tetromino->instantFall();
				break;

			case 'T':
				setHeldBlock(new_tetromino);
				break;

			case 27:	// Esc
				break;
			}
		}
	}
}


void Game::checkFullLane(Tetromino*& new_tetromino) {

	// Limiting the deleting lanes
	short under_lane = 19;

	for (auto& block : new_tetromino->getBlock()) {
		if (under_lane < block.apparent.Y)
			under_lane = block.apparent.Y;
	}

	// Find the full lanes
	//
	//      lane loop (max to 0)
	//         v
	// [ X ] [ V ] [ X ] [ V ] [ V ] [ X ] [ X ]
	//   ^
	// deleting lane = lane loop + going down level
	//
	short going_down_level = 0;
	short deleting_lane = under_lane;

	for (short lane = under_lane; lane >= 0; lane--) {
		if (_isLaneFull(lane)) {
			going_down_level++;
			_whitenLane(lane);
		}
		else {
			if (going_down_level != 0) {
				_shiftLane(deleting_lane, going_down_level);
			}
			deleting_lane--;
		}
	}

	if (going_down_level == 0)
		return;				// Avoid running the next for loop without any changes

	for (short lane = deleting_lane; lane >= 0; lane--) {
		_emptyLane(lane);
	}

	// Each deleting lane will get you a score
	switch (going_down_level) {

	case 1:
		score += 3 * level.level;
		requiring_bar++;
		break;

	case 2:
		score += 10 * level.level;
		requiring_bar += 3;
		break;

	case 3:
		score += 18 * level.level;
		requiring_bar += 5;
		break;

	case 4:
		score += 32 * level.level;
		requiring_bar += 8;
		break;
	}

	_whitenBar(requiring_bar);
	setRequiringBar();

	Sleep(20);

	drawLevel();
	drawScore();
	drawRequiringBar();
	drawHeldBlock();
	redrawGame();
}



void Game::resetBlock(Tetromino*& new_tetromino) {
	setBlock(new_tetromino);
	setWaitingBlocks();
	drawWaitingBlocks();
}
