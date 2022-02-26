#ifndef _WALL_FOLLOWING
#define _WALL_FOLLOWING


#include "MazeSolving.h"


struct FollowingMark {
	bool up_left = false, down_left = false, down_right = false, up_right = false;
};


char getLeftSide(char& direction);
char getRightSide(char& direction);


class WallFollowing : public MazeSolving {

protected:
	FollowingMark bitmap[16][16];
	bool left_follow;
	char prev_direction;

public:
	WallFollowing(bool left_follow) : MazeSolving() {
		this->left_follow = left_follow;
		prev_direction = 0;
	}

	char findPath(Maze& maze);

	bool haveWallAhead(Maze& maze);
	bool haveWallOnLeft(Maze& maze);
	bool haveWallOnRight(Maze& maze);
	void moveAhead();
	void setAndDrawFollowingWay();
};


#endif