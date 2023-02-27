#pragma once
#include "State.hpp"
#include <vector>
#include <queue>
#include "compareCellCoins.hpp"
#include "Cell.hpp"

using namespace std;
using namespace global;

class State;
class Pacman
{
private:
	State* currentState;
	int row, col;
	bool isChasing, isCollecting, isRunning;
	int coinAmount;
	vector<Cell*> coinPath;
	float distFromGhost;
	int timer;
	int ghostPos[3][2];
	int TIMERAMOUNT;

public:
	Pacman();
	~Pacman();
	Pacman(Global& world);
	int getDistFromGhost() { return distFromGhost; }
	void setCurrentState(State* pNewState) { currentState = pNewState; }
	State* getCurrentState() { return currentState; }
	void DrawMe(int maze[global::MSZ][global::MSZ]);
	void Execute();
	void setIsChasing(bool value) { isChasing = value; }
	void setIsCollecting(bool value) { isCollecting = value; }
	void setIsRunning(bool value) { isRunning = value; }
	bool getIsChasing() { return isChasing; }
	bool getIsCollecting() { return isCollecting; }
	bool getIsRunning() { return isRunning; }
	void setRow(int r) { row = r; }
	void setCol(int c) { col = c; }
	int getRow() { return row; }
	int getCol() { return col; }
	void setCoin() { coinAmount++; }
	int getCoin() { return coinAmount; }
	void runCoinAStar(priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pqC, vector<Cell>&grays, Global& world, int kind);
	void runCoinsSearch(Global& world, int kind);
	bool checkCoinNeighbor(int rows, int cols, Cell* pCorr, priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pqA, vector<Cell> &black, vector<Cell> &grays, int dir, Global& world,int kind);
	double distant(int x1, int y1, int x2, int y2);
	void UpdatePQCoin(Cell* neighbor,priority_queue <Cell*, vector<Cell*>, CompareCellCoins>& pq);
	float getGhostDist(){ return distFromGhost; }
	void setGhostDist(Global &world);
	void setTimer(int t) { timer = t; }
	void addTimer() { timer = timer + 1; }
	int getTimer() { return timer; }
	void setPacmanPos(Global& world);
	void checkCoin(Global& world);
	bool BfsNeighbor(Global& world, int row, int col, Cell* pcurrent, vector<Cell*>& grays);
	void BfsChase(Global& world);
	void setTimerAmount(int num) { TIMERAMOUNT = TIMERAMOUNT + num; }
	int getTimerAmount() { return TIMERAMOUNT; }

};

