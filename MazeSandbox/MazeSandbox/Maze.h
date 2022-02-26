#ifndef _MAZE_H
#define _MAZE_H


#include "Drawing.h"


#define BITMAP(pos) bitmap[pos.Y][pos.X]
#define BITMAP_S(pos, shift_x, shift_y) bitmap[pos.Y + shift_y][pos.X + shift_x]
#define MAP(pos) maze.map[pos.Y][pos.X]
#define MAP_S(pos, shift_x, shift_y) maze.map[pos.Y + shift_y][pos.X + shift_x]

#define DIRECTION_GROUP _up, _left, _down, _right
#define DIRECTION_GROUP_REFERENCE bool& _up, bool& _left, bool& _down, bool& _right
#define SET_DIRECTION_VALUE(value) _up = _left = _down = _right = value


enum class UnitState {
	empty,
	block,
	start_point,
	finish_point
};


struct Unit {
	bool up = false;
	bool left = false;
	bool right = false;
	bool down = false;

	UnitState state = UnitState::empty;
};


class Maze {

protected:
	bool winnable = false;
	bool cycling = false;

	std::string maze_name;

	// This will use apparent coordinates (16 x 16)
	// The real coordinates is the position on the console application
	COORD start_pos{ -1, -1 };
	COORD finish_pos{ -1, -1 };

	Unit map[16][16];

	// friend parameters
	friend class Drawing;

	friend class RandomDFSMaze;
	friend class RandomKruskalMaze;
	friend class RandomPrimMaze;
	friend class AldousBroderMaze;
	friend class HuntAndKillMaze;
	friend class BinaryTreeMaze;
	friend class EllerMaze;

	friend class DFSSolver;
	friend class BFSSolver;
	friend class DijkstraSolver;
	friend class AStarSolver;

	friend class WallFollowing;
	friend class Pledge;

public:
	Maze();
	~Maze();

	// Setter and getter
	void setState(COORD&);
	void setMazeName(std::string value) { maze_name = value; }
	std::string getMazeName() { return maze_name; }

	// Set the connection
	void connect(COORD&, COORD&);

	// Draw the interface
	void drawUnit(COORD&);
	void drawWholeMap();
	void setValidatingBoxValue();

	// Draw the starting point and ending point
	void drawStartPoint(COORD&);
	void drawFinishPoint(COORD&);
	void redrawStartAndFinish();

	void instantlyDrawUnit(COORD&);				// Draw the unit without changing anything before data
	void instantlyDrawEdge(COORD&, COORD&);		// Like we change the colour first, then the drawing will use that one
	void instantlyDrawSquare(COORD&);

	// Clear the interface
	void clearAroundUnit(COORD&);
	void clearWholeMap(bool effect);		// if true: the effect will be added, if false: just immediately clear

	void drawCursor(COORD&, COORD&);
	void drawCursor(COORD&);
	void deleteCursor(COORD&);

	// Save and load
	void saveGame();
	void loadGame();
};


// Convert the apparent coordinates to the real one
COORD toRealCoord(COORD);
COORD toApparentCoord(COORD);


#endif
