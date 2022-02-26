#include "RandomKruskalMaze.h"


COORD RandomKruskalMaze::findParent(COORD& pos) {
	if (not equalCoord(BITMAP(pos).parent, pos)) {
		BITMAP(pos).parent = findParent(BITMAP(pos).parent);	// Or just return findParent(...)
	}
	return BITMAP(pos).parent;
}


#pragma warning(push)
#pragma warning(disable:6385)
#pragma warning(disable:6386)
void RandomKruskalMaze::generate(Maze& maze, COORD& pos) {

	short count = 16 * 16 - 1;

	while (count != 0) {

		// 1) Let's randomize!
		// For the number 31: this will connect horizontally or vertically depending on the value
		// E.g. (x, y) = (4, 10) => [4][10] <-> [4][11]
		// E.g. (x, y) = (19, 10) => [4][10] <-> [5][10] (19 - 15 = 4)

		COORD kru_coord{};

		kru_coord.X = rand() % 31;
		kru_coord.Y = rand() % 16;


		// 1.1) If the kruskal edge has value[n][8] where n >= 16 => invalid edge
		if ((kru_coord.Y == 15) and (kru_coord.X >= 15))
			continue;


		// 1.2) Found unused edge! Now check it
		COORD curr1 = kru_coord, curr2 = kru_coord;
		// For two horizontal adjacent units
		if (kru_coord.X < 15) {
			curr2.X++;
		}
		// For two vertical adjacent units
		else {
			curr1.X -= 15;
			curr2.X -= 15;
			curr2.Y++;
		}



		// 2) If both has different parent => the edge is valid to add
		// This is quite hard to understand without knowing Kruskal's algorithm first
		// If you know but still don't understand, please check the file for deatails (If I have written :b)

		COORD parent_pos1 = findParent(BITMAP(curr1).parent);
		COORD parent_pos2 = findParent(BITMAP(curr2).parent);

		// 2.1) This time, use 'parent', NEVER USE 'coord'
		KruskalUnit* P1 = &BITMAP(parent_pos1);
		KruskalUnit* P2 = &BITMAP(parent_pos2);

		if (not equalCoord(parent_pos1, parent_pos2)) {

			// Choose the unit to become a parent. The one with the lower unit will have a new parent
			if ((*P1).rank > (*P2).rank)
				(*P2).parent = parent_pos1;

			else if ((*P1).rank < (*P2).rank)
				(*P1).parent = parent_pos2;

			else {	// There is a case that both rank are the same. In this one, we just choose one of them
				(*P2).parent = parent_pos1;
				(*P1).rank++;
			}

			// Done! Now we just draw them
			MAP(curr1).state = UnitState::block;
			MAP(curr2).state = UnitState::block;

			Console::setColour(white * 16);
			maze.connect(curr1, curr2);
			maze.instantlyDrawUnit(curr1);
			maze.instantlyDrawEdge(curr1, curr2);
			maze.instantlyDrawUnit(curr2);

			count--;
		}

		Sleep(30);
	}

	maze.cycling = false;
	maze.drawCursor(pos);
}
