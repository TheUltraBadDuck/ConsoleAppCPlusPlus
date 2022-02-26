#ifndef _PLEDGE
#define _PLEDGE


#include "WallFollowing.h"


class Pledge : public WallFollowing {

protected:
	short rotation = 0;

public:
	Pledge(bool left_follow) : WallFollowing(left_follow) {

	}

	char getDirectionTowardFinish(Maze& maze);

	char findPath(Maze& maze);
};


#endif
