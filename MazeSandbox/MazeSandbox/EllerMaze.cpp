#include "EllerMaze.h"


// For STEP 4: If connect, two groups' bitmap values must share the same value
void EllerMaze::mergeBitmap(short bit1, short bit2) {

	if (bit1 == bit2)
		return;

	// We should choose the smaller number
	short min_bit = min(bit1, bit2);
	short max_bit = max(bit1, bit2);

	// The group will the bigger number will be the same as the one with smaller number
	for (short i = 0; i < 16; i++) {
		if (bitmap[i][0] == -1)
			return;

		for (short j = 0; j < 16; j++) {
			if (bitmap[i][j] == max_bit) {
				bitmap[i][j] = min_bit;
			}
		}
	}
}


void EllerMaze::generate(Maze& maze, COORD& pos) {

	COORD curr{};
	COORD prev{};
	short ungroup_bit_value = 0;	// For ungrouping bitmap

	while (curr.Y < 15) {

	// Step 1: create the group for the first line
	// E.g.: From {0 1 2 3 4 5 6 7 8 9} to {0 0 0 0 4 5 5 5 8 8}
	// 
	// pos_storage: {0: {0, 1, 2, 3}, 4: {4}, 5: {5, 6, 7}, 8: {8, 9}}

		curr.X = 0;
		prev = curr;

		if (BITMAP(curr) == -1)
			BITMAP(curr) = ungroup_bit_value++;
		pos_storage[BITMAP(curr)].push_back(0);		// Or [0] := (position x = 0)

		for (curr.X = 1; curr.X < 16; curr.X++) {

			if (BITMAP(curr) == -1)
				BITMAP(curr) = ungroup_bit_value++;

			// For the first lane: there is no restriction in not connecting.
			// For the rest: ignore the compulsory wall positions
			if ((left_wall_poss.size() > 0) and left_wall_poss.back() == curr.X - 1) {
				left_wall_poss.pop_back();
			}
			// If there is no restriction:  We can randomize whether to connect or not
			else {
				if (rand() % 2) {	// If connects
					Console::setColour(white * 16);
					mergeBitmap(BITMAP(prev), BITMAP(curr));
					maze.connect(prev, curr);
					maze.instantlyDrawEdge(prev, curr);
					BITMAP(curr) = BITMAP_S(curr, -1, 0);
				}
			}

			pos_storage[BITMAP(curr)].push_back(curr.X);
			
			maze.instantlyDrawUnit(prev);
			maze.instantlyDrawUnit(curr);

			Sleep(30);

			prev = curr;
		}

	// Step 2: connect below
	// E.g.: {0 0 0 0 4 5 5 5 8 8}
	//    to {0 0 _ 0 4 5 _ 5 8 _}
	//
	// The role of 'pos_storage' is to make sure there is at least one connection below each group
	// 
	// con_above_pos_storage: {0: {0, 1, 3}, 5: {5, 7}} (note the single connection of group 4 and 8 and the lack of those elements)

		curr.X = 0;
		curr.Y++;
		for (auto& iter : pos_storage) {

			short i = 0;
			curr.X = iter.second[i];
			// To stop the loop, there is AT LEAST one connection below (connecting_below = true)
			// And the 'con_pos_x' will loop back if it reaches to the right
			if (iter.second.size() > 1) {

				bool con_below = false;
				while ((not con_below) or (curr.X != iter.second[0])) {

					if (rand() % 2) { // If connects

						COORD temp = curr;
						temp.Y--;
						BITMAP(curr) = BITMAP(temp);

						Console::setColour(white * 16);
						maze.connect(temp, curr);
						maze.instantlyDrawEdge(temp, curr);
						maze.instantlyDrawUnit(temp);
						maze.instantlyDrawUnit(curr);

						con_below = true;
						con_above_pos_storage[BITMAP(curr)].push_back(curr.X);
					}

					if (++i == iter.second.size())
						i = 0;

					curr.X = iter.second[i];

					Sleep(30);
				}
			}
			// If the group only have one unit => instantly connect below
			else {
				COORD temp = curr;
				temp.Y--;
				BITMAP(curr) = BITMAP(temp);

				Console::setColour(white * 16);
				maze.connect(temp, curr);
				maze.instantlyDrawEdge(temp, curr);
				maze.instantlyDrawUnit(temp);
				maze.instantlyDrawUnit(curr);

				con_above_pos_storage[BITMAP(curr)].push_back(curr.X);

				Sleep(30);
			}

			prev = curr;
		}

		pos_storage.clear();

	// Step 3: Create a wall first
	// E.g.: {0 0 0 0 4 5 5 5 8 8}
	//    to {0|0 _|0 4 5|_ 5 8 _}
	// 
	// Note: Look at group 0 for the example. There is 3 connection above, => there are 2 compulsory walls between each of the couple of them (3 - 1 = 2)
	// If there is only 1 connection above (such as group 4 or group 8) => no compulsory wall is created
	// left_wall_poss: {0, 2, 5}, or {0, 1, 5}, or {0, 2, 6}

		for (auto& iter : con_above_pos_storage) {

			std::sort(iter.second.begin(), iter.second.end());

			for (short i = 1; i < iter.second.size(); i++) {

				short distance = iter.second[i] - iter.second[i - 1];
				if (distance == 1) {
					left_wall_poss.push_back(iter.second[i - 1]);
				}
				else {
					left_wall_poss.push_back(rand() % distance + iter.second[i - 1]);
				}
			}
		}
		con_above_pos_storage.clear();
		std::sort(left_wall_poss.begin(), left_wall_poss.end(), [](short& a, short& b) {return a > b; });
	}

	// Step 4: the last line will be set
	// If two groups are different, they have to be merged
	curr.X = 0;
	prev = curr;

	if (BITMAP(curr) == -1)
		BITMAP(curr) = ungroup_bit_value++;
	pos_storage[BITMAP(curr)].push_back(0);		// Or [0] := (position x = 0)

	for (curr.X = 1; curr.X < 16; curr.X++) {

		if (BITMAP(curr) == -1)
			BITMAP(curr) = ungroup_bit_value++;

		if ((left_wall_poss.size() > 0) and left_wall_poss.back() == curr.X - 1) {
			left_wall_poss.pop_back();
		}
		else {
			if (BITMAP(prev) != BITMAP(curr)) {
				Console::setColour(white * 16);
				mergeBitmap(BITMAP(prev), BITMAP(curr));
				maze.connect(prev, curr);
				maze.instantlyDrawEdge(prev, curr);
				BITMAP(curr) = BITMAP_S(curr, -1, 0);
			}
		}

		pos_storage[BITMAP(curr)].push_back(curr.X);

		maze.instantlyDrawUnit(prev);
		maze.instantlyDrawUnit(curr);

		Sleep(30);

		prev = curr;
	}

	maze.drawCursor(pos);
}
