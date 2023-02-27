#pragma once

#include "Ghost.hpp"


Ghost::Ghost()
{
}

Ghost::~Ghost()
{
}

void Ghost::runGhostAStar(priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pqC, vector<Cell>& grays, Global& world)
{
	vector<Cell> black;
	vector<Cell>::iterator itGray;
	Cell* pCurrent;
	bool solutionFound2 = false;

	while (!solutionFound2)
	{
		if (pqC.empty())
		{
			return;
		}
		pCurrent = pqC.top();
		// paint pCurrent black and extract it from pq
		pqC.pop();
		itGray = find(grays.begin(), grays.end(), *pCurrent);
		if (itGray == grays.end()) // pCurrent wasn't found
		{
			std::cout << "ERROR: pCurrent wasn't found\n";
			return;
		}
		grays.erase(itGray);
		black.push_back(*pCurrent);
		const int r = pCurrent->getRow();
		const int c = pCurrent->getCol();
		if (world.maze[r][c - 1] == global::SPACE || world.maze[r][c - 1] == global::PAC ||  world.maze[r][c - 1] == global::GRAY)
			solutionFound2 = checkGhostNeighbor(r, c - 1, pCurrent, pqC, black, grays, world);
		if ((world.maze[r - 1][c] == global::SPACE || world.maze[r - 1][c] == global::PAC || world.maze[r - 1][c] == global::GRAY) && !solutionFound2)
			solutionFound2 = checkGhostNeighbor(r - 1, c, pCurrent, pqC, black, grays, world);
		if ((world.maze[r][c + 1] == global::SPACE || world.maze[r][c + 1] == global::PAC || world.maze[r][c + 1] == global::GRAY) && !solutionFound2)
			solutionFound2 = checkGhostNeighbor(r, c + 1, pCurrent, pqC, black, grays, world);
		if ((world.maze[r + 1][c] == global::SPACE || world.maze[r + 1][c] == global::PAC || world.maze[r + 1][c] == global::GRAY) && !solutionFound2)
			solutionFound2 = checkGhostNeighbor(r + 1, c, pCurrent, pqC, black, grays, world);

	}
}

void Ghost::runGhostSearch(Global& world)
{
	while (!pacPath.empty()) {
		pacPath.pop_back();
	}
	std::vector<Cell> coinGray;
	std::priority_queue<Cell*, std::vector<Cell*>, CompareCellCoins> pqG;
	int closest = 0;
	//pac pos
	Cell* pc = new Cell(row, col);;
	pc->computeH(pacRow, pacCol);
	pqG.push(pc);
	coinGray.push_back(*pc);

	runGhostAStar(pqG, coinGray, world);
}

bool Ghost::checkGhostNeighbor(int rows, int cols, Cell* pCorr, priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pqA, vector<Cell>& black, vector<Cell>& grays, Global& world)
{
	vector<Cell>::iterator itGray;
	vector<Cell>::iterator itBlack;
	Cell* neighbor = new Cell(rows, cols, pCorr);//and father cell (added).
	if (cols <= 0 || cols >= global::MSZ)
		return false;
	if (world.maze[rows][cols] == global::PAC) {
		while (neighbor->getParent()) {
			pacPath.push_back(neighbor);
			neighbor = neighbor->getParent();
		}
		return true;
	}
	neighbor->computeH(pacRow, pacCol);
	neighbor->setF();

	itGray = std::find(grays.begin(), grays.end(), *neighbor);
	itBlack = std::find(black.begin(), black.end(), *neighbor);

	if (itGray == grays.end() && itBlack == black.end()) {
		pqA.push(neighbor);
		grays.push_back(*neighbor);
	}
	else if (itGray != grays.end()) {
		if (neighbor->getF() < itGray->getF()) {
			*itGray = *neighbor;
			UpdatePQGhost(neighbor, pqA);
		}
	}
	return false;
}

double Ghost::distant(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Ghost::UpdatePQGhost(Cell* neighbor, priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pq)
{
	std::vector <Cell*>tmp;
	Cell* p;
	p = pq.top();
	while (!pq.empty() && !((*p) == *neighbor))
	{
		tmp.push_back(p);
		pq.pop();
		p = pq.top();
	}
	if (pq.empty())
	{
		std::cout << "Error! PQ is empty in UpdatePQ\n";
		exit(1);
	}
	// p points to an element in pq that is equals pNeighbor
	pq.pop();
	pq.push(neighbor);
	while (!tmp.empty())
	{
		pq.push(tmp.back());
		tmp.pop_back();
	}
}

void Ghost::setGhostPos(Global& world,bool isChasing) {
	Cell* nextStep;
	if (!pacPath.empty()&& !isChasing) {
		nextStep = pacPath.back();
		world.maze[row][col] = SPACE;
		row = nextStep->getRow();
		col = nextStep->getCol();
		pacPath.pop_back();
	}
	else if (!pacPath.empty() && isChasing) {
		nextStep = pacPath.back();
		int tmpRow = row + (row - nextStep->getRow());
		int tmpCol = col + (col - nextStep->getCol());
		world.maze[row][col] = SPACE;
		if (world.maze[tmpRow][tmpCol] == WALL || world.maze[tmpRow][tmpCol] == GRAY) {
			if (tmpRow == row) {
				if (world.maze[tmpRow + 1][col] == WALL || world.maze[tmpRow + 1][col] == GRAY) {
					tmpRow = tmpRow - 1;
					row = tmpRow;
				}
				else {
					tmpRow = tmpRow + 1;
					row = tmpRow;
				}
			}
			else if (tmpCol == col) {
				if (world.maze[row][tmpCol + 1] == WALL || world.maze[row][tmpCol] == GRAY) {
					tmpCol = tmpCol - 1;
					col = tmpCol;
				}
				else {
					tmpCol = tmpCol + 1;
					col = tmpCol;
				}

			}
			if (tmpRow > MSZ)
				tmpRow = MSZ;
			else if (tmpRow < 0)
				tmpRow = 0;
			if (tmpCol < 0)
				tmpCol = 0;
			else if (tmpCol > MSZ)
				tmpCol = MSZ;
		}
		else {
			row = tmpRow;
			col = tmpCol;
		}

		pacPath.pop_back();
	}
	world.maze[row][col] = GRAY;
}

void Ghost::checkEaten(Global& world, int pacRow, int pacCol)
{
	if (row == pacRow && col == pacCol) {
		beenEaten = true;
		world.maze[row][col] = PAC;
	}
}

 
