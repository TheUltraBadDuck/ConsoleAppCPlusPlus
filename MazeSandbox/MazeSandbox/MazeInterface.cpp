#include "MazeInterface.h"


MazeInterface::MazeInterface() {
	drawing.drawInterface();
}


MazeInterface::~MazeInterface() {

}


void MazeInterface::runProject() {

	bool change = false;
	COORD previous_pos{};
	short returned_choice = 0;

	drawing.drawGeneratorIcon(0);
	drawing.drawSolverIcon(0);
	drawing.drawAllButtons();
	maze.setValidatingBoxValue();
	maze.drawCursor(current_pos);

	while (true) {

		previous_pos = current_pos;
		char c = toupper(_getch());

		switch (c) {

		// Movement keyboard
		case 'W':
			if (current_pos.Y > 0) {
				current_pos.Y--;
				change = true;
			}
			break;

		case 'A':
			if (current_pos.X > 0) {
				current_pos.X--;
				change = true;
			}
			break;

		case 'S':
			if (current_pos.Y < 15) {
				current_pos.Y++;
				change = true;
			}
			break;

		case 'D':
			if (current_pos.X < 15) {
				current_pos.X++;
				change = true;
			}
			break;

		// Clear unit
		case '0':
			drawing.drawButton(0);
			maze.clearAroundUnit(current_pos);
			maze.drawCursor(current_pos);
			maze.setValidatingBoxValue();
			break;

		// Draw by trailing
		case '1':
			move_and_draw = not move_and_draw;
			drawing.drawButton(1);
			break;

		// Draw start point
		case '2':
			drawing.drawButton(2);
			maze.drawStartPoint(current_pos);
			maze.setValidatingBoxValue();
			break;

		// Draw finish point
		case '3':
			drawing.drawButton(3);
			maze.drawFinishPoint(current_pos);
			maze.setValidatingBoxValue();
			break;

		// Generate maze
		case '4':
			drawing.drawButton(4);
			returned_choice = drawing.operateGeneratorChoice();
			// Choose nothing
			if (returned_choice == -1) {
				drawing.drawButton(4);
				break;
			}

			maze.clearWholeMap(false);
			generateMaze(returned_choice);
			maze.setValidatingBoxValue();

			drawing.drawButton(4);
			break;

		// Solve maze
		case '5':
			drawing.drawButton(5);
			returned_choice = drawing.operateSolverChoice();
			// Choose nothing
			if (returned_choice == -1) {
				drawing.drawButton(5);
				break;
			}

			solveMaze(returned_choice);
			maze.setValidatingBoxValue();
			drawing.drawButton(5);
			break;

		// Save game
		case '8':
			drawing.drawButton(6);
			maze.setMazeName(drawing.operateSaveMenuChoice());
			maze.saveGame();
			maze.drawWholeMap();
			drawing.drawButton(6);
			break;

		// Load game
		case '9':
			drawing.drawButton(7);
			maze.setMazeName(drawing.operateLoadMenuChoice());
			maze.loadGame();
			maze.drawWholeMap();
			maze.setValidatingBoxValue();
			drawing.drawButton(7);
			break;

		case 27:
			drawing.drawButton(8);
			maze.clearWholeMap(true);
			maze.setValidatingBoxValue();
			maze.drawCursor(current_pos);
			drawing.drawButton(8);
			break;

		default:
			break;
		}

		if (change == true) {
			if (move_and_draw) {
				maze.connect(previous_pos, current_pos);
				maze.drawUnit(previous_pos);
				maze.drawUnit(current_pos);
			}
			change = false;
		}

		maze.drawCursor(previous_pos, current_pos);
	}
}


void MazeInterface::generateMaze(short& returned_choice) {

	MazeGenerator* new_maze;

	switch (returned_choice) {

	case 0: new_maze = new RandomDFSMaze;		break;
	case 1: new_maze = new RandomKruskalMaze;	break;
	case 2: new_maze = new RandomPrimMaze;		break;
	case 3: new_maze = new AldousBroderMaze;	break;
	case 4: new_maze = new HuntAndKillMaze;		break;
	case 5: new_maze = new BinaryTreeMaze;		break;
	case 6:	new_maze = new EllerMaze;			break;
	default: return; break;
	}

	new_maze->generate(maze, current_pos);
	delete new_maze;
}

bool chooseLeftFollow() {
	while (true) {
		char _c = (toupper(_getch()));
		switch (_c) {
		case 'L': case 'Z':
			return true;
			break;
		case 'R': case 'X':
			return false;
			break;
		}
	}
}

void MazeInterface::solveMaze(short& returned_choice) {

	char compiler_status = 0;
	PathFinding* new_solver_1;
	MazeSolving* new_solver_2;

	switch (returned_choice) {

	case 0:
		new_solver_1 = new DFSSolver;
		new_solver_2 = NULL;
		break;
	case 1:
		new_solver_1 = new BFSSolver;
		new_solver_2 = NULL;
		break;
	case 2:
		new_solver_1 = new DijkstraSolver;
		new_solver_2 = NULL;
		break;
	case 3:
		new_solver_1 = new AStarSolver;
		new_solver_2 = NULL;
		break;

	case 4:
		drawing.writeCustom("Press [L]/[Z] for left following, [R]/[X] for right following");
		new_solver_1 = NULL;
		new_solver_2 = new WallFollowing(chooseLeftFollow());
		drawing.clearInfo();
		break;

	case 5:
		drawing.writeCustom("Press [L]/[Z] for left following, [R]/[X] for right following");
		new_solver_1 = NULL;
		new_solver_2 = new Pledge(chooseLeftFollow());
		drawing.clearInfo();
		break;


	default:
		new_solver_1 = NULL;
		new_solver_2 = NULL;
		break;
	}

	if (new_solver_1 != NULL) {
		compiler_status = new_solver_1->findPath(maze);
		delete new_solver_1;
	}
	if (new_solver_2 != NULL) {
		compiler_status = new_solver_2->findPath(maze);
		delete new_solver_2;
	}

	drawing.writeCompilingStatus(compiler_status);
	(void)_getch();
	drawing.clearInfo();
	maze.drawWholeMap();
}
