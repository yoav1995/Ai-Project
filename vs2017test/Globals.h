#pragma once
#include <vector>
#include <queue>


namespace global {
	using ROOM = struct {
		int startRow;
		int startCol;
		int width;
		int height;
		int midRow;
		int midCol;
	};

	using COIN = struct {
		int posRow;
		int posCol;
		bool beenFound;
	};

	const int WALL = 0;
	const int SPACE = 1;
	const int START = 2;
	const int TARG = 3;
	const int GRAY = 4;
	const int BLACK = 5;
	const int ROOMAMOUNT = 8;
	const int PAC = 7;
	const int COINS = 8;
	const int PATH = 9;
	const int MSZ = 100;
	const int COINSAMAOUNT = 18;

}

class Global {
public:
	int maze[global::MSZ][global::MSZ];
	global::COIN coins[global::COINSAMAOUNT];
	global::ROOM rooms[global::ROOMAMOUNT];
};

