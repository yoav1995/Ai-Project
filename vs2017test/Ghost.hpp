#pragma once
#include "State.hpp"
#include "Cell.hpp"


class Ghost
{
private:
	int row, col;
	vector<Cell*> pacPath;
	int pacRow, pacCol;
	bool beenEaten;

	
public:
	Ghost();
	~Ghost(); 
	void setRow(int r) { row = r; }
	void setCol(int c) { col = c; }
	void setPacRow(int r) { pacRow = r; }
	void setPacCol(int c) { pacCol = c; }
	int getRow() { return row; }
	int getCol() { return col; }
	int getPacRow() { return pacRow; }
	int getPacCol() { return pacCol; }
	void setRow(bool e) { beenEaten = e; }
	bool getEaten() { return beenEaten; }
	void runGhostAStar(priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pqC, vector<Cell>& grays, Global& world);
	void runGhostSearch(Global& world);
	bool checkGhostNeighbor(int rows, int cols, Cell* pCorr, priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pqA, vector<Cell>& black, vector<Cell>& grays, Global& world);
	double distant(int x1, int y1, int x2, int y2);
	void UpdatePQGhost(Cell* neighbor, priority_queue <Cell*, vector<Cell*>, CompareCellCoins>& pq);
	void setGhostPos(Global& world, bool isChasing);
	void checkEaten(Global& world,int pacRow, int pacCol);
};

