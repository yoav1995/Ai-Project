#include "Pacman.hpp"
#include "CollectCoins.hpp"
#include "glut.hpp"
#include "compareCellCoins.hpp"
#include <queue>
#include <vector>


using namespace std;
using namespace global;

int Trow, Tcol;


Pacman::Pacman()
{
	isChasing = false;
	isCollecting = false;
	isRunning = false;
	coinAmount = 0;
	distFromGhost = 10000;
	timer = 0;
	TIMERAMOUNT = 45;
}

Pacman::Pacman(Global& world)
{
	isChasing = false;
	isCollecting = false;
	isRunning = false;
	coinAmount = 0;
	currentState = new CollectCoins();
	currentState->OnStateEnter(this,world);
	distFromGhost = 10000;
	timer = 0;
	TIMERAMOUNT = 45;
}

Pacman::~Pacman()
{
}


void Pacman::DrawMe(int maze[global::MSZ][global::MSZ] )
{
	for (int i = 0; i < global::MSZ;i++) {
		for (int j = 0;j < global::MSZ;j++) {
			if (maze[i][j] == global::PAC)
				maze[i][j] = global::SPACE;
		}
		maze[this->row][this->col] = global::PAC;
	}
}

void Pacman::Execute()
{
}

void Pacman:: UpdatePQCoin(Cell* neighbor, priority_queue <Cell*, vector<Cell*>, CompareCellCoins>& pq) {
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

void Pacman::setGhostDist(Global &world)
{
	int amount = 0;
	for (int i = 0;i<global::MSZ; i++) {
		for (int j = 0;j<global::MSZ; j++) {
			if (world.maze[i][j] == global::GRAY) {
				ghostPos[amount][0] = i;
				ghostPos[amount][1] = j;
				amount++;
			}
		}
	}
	double shortest;
	distFromGhost = distant(row, col, ghostPos[0][0], ghostPos[0][1]);
	for (int i = 1;i < 3;i++) {
		shortest = distant(row, col, ghostPos[i][0], ghostPos[i][1]);
		if (shortest < distFromGhost)
			distFromGhost = shortest;
	}
}

bool Pacman::checkCoinNeighbor(int rows, int cols, Cell* pCorr, priority_queue<Cell*, vector< Cell* >, CompareCellCoins> & pqA, vector<Cell>& black, vector<Cell>& grays, int dir, Global& world,int kind) {
	vector<Cell>::iterator itGray;
	vector<Cell>::iterator itBlack;
	Cell* neighbor = new Cell(rows, cols, pCorr, dir);//and father cell (added).
	if (cols <= 0 || cols >= global::MSZ)
		return false;
	if (kind == global::COINS) {
		if (world.maze[rows][cols] == global::COINS) {
			while (neighbor->getParent()) {
				coinPath.push_back(neighbor);
				neighbor = neighbor->getParent();
			}
			return true;
		}
	}
	else if (kind == global::GRAY) {
		if (world.maze[rows][cols] == global::GRAY) {
			while (neighbor->getParent()) {
				coinPath.push_back(neighbor);
				neighbor = neighbor->getParent();
			}
			return true;
		}	
	}
	
	neighbor->computeH(Trow, Tcol);
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
			UpdatePQCoin(neighbor, pqA);
		}
	}
	return false;
}


void Pacman::runCoinAStar(priority_queue<Cell*, vector<Cell*>, CompareCellCoins>& pqC, vector<Cell>& grays, Global& world,int kind) {
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
		if (kind == global::COINS) {
			if (world.maze[r][c - 1] == global::SPACE || world.maze[r][c - 1] == global::COINS)
				solutionFound2 = checkCoinNeighbor(r, c - 1, pCurrent, pqC, black, grays, 1, world,kind);
			if ((world.maze[r - 1][c] == global::SPACE || world.maze[r - 1][c] == global::COINS) && !solutionFound2)
				solutionFound2 = checkCoinNeighbor(r - 1, c, pCurrent, pqC, black, grays, 3, world, kind);
			if ((world.maze[r][c + 1] == global::SPACE || world.maze[r][c + 1] == global::COINS) && !solutionFound2)
				solutionFound2 = checkCoinNeighbor(r, c + 1, pCurrent, pqC, black, grays, 2, world, kind);
			if ((world.maze[r + 1][c] == global::SPACE || world.maze[r + 1][c] == global::COINS) && !solutionFound2)
				solutionFound2 = checkCoinNeighbor(r + 1, c, pCurrent, pqC, black, grays, 4, world, kind);
		}
		else {
			if (world.maze[r][c - 1] == global::SPACE || world.maze[r][c - 1] == global::GRAY || world.maze[r][c - 1] == global::COINS)
				solutionFound2 = checkCoinNeighbor(r, c - 1, pCurrent, pqC, black, grays, 1, world, kind);
			if ((world.maze[r - 1][c] == global::SPACE || world.maze[r - 1][c] == global::GRAY || world.maze[r - 1][c] == global::COINS) && !solutionFound2)
				solutionFound2 = checkCoinNeighbor(r - 1, c, pCurrent, pqC, black, grays, 3, world, kind);
			if ((world.maze[r][c + 1] == global::SPACE || world.maze[r][c + 1] == global::GRAY || world.maze[r][c + 1] == global::COINS) && !solutionFound2)
				solutionFound2 = checkCoinNeighbor(r, c + 1, pCurrent, pqC, black, grays, 2, world, kind);
			if ((world.maze[r + 1][c] == global::SPACE || world.maze[r + 1][c] == global::GRAY || world.maze[r + 1][c] == global::COINS) && !solutionFound2)
				solutionFound2 = checkCoinNeighbor(r + 1, c, pCurrent, pqC, black, grays, 4, world, kind);
		}
		

	}
}

double Pacman::distant(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void Pacman::runCoinsSearch(Global& world,int kind)
{
	while (!coinPath.empty()) {
		coinPath.pop_back();
	}
	std::vector<Cell> coinGray;
	std::priority_queue<Cell*, std::vector<Cell*>, CompareCellCoins> pqC;
	int closest = 0;
	if (kind == global::COINS) {
		for (int i = coinAmount + 1;i < global::COINSAMAOUNT;i++) {
			if ((distant(row, col, world.coins[closest].posRow, world.coins[closest].posCol) > distant(row, col, world.coins[i].posRow, world.coins[i].posCol)) && world.coins[i].posRow != 0 && world.coins[i].posCol != 0 && world.coins->beenFound == false)
				closest = i;
		}
		Trow = world.coins[closest].posRow;
		Tcol = world.coins[closest].posCol;
	}
	else if (kind == global::GRAY) {

		int amount = 0;
		int pos[3][2];
		for (int i = 0;i < global::MSZ; i++) {
			for (int j = 0;j < global::MSZ; j++) {
				if (world.maze[i][j] == global::GRAY) {
					pos[amount][0] = i;
					pos[amount][1] = j;
					amount++;
				}
			}
		}

		double shortest;
		distFromGhost = distant(row, col, pos[0][0], pos[0][1]);
		for (int i = 1;i < 3;i++) {
			shortest = distant(row, col, pos[i][0], pos[i][1]);
			if (shortest < distFromGhost) {
				Trow = pos[i][0];
				Tcol = pos[i][1];
			}
				
		}		
	}
	Cell* pc = new Cell(row, col);;
	pc->computeH(Trow, Tcol);
	pqC.push(pc);
	coinGray.push_back(*pc);

	runCoinAStar(pqC, coinGray, world, kind);
}

void Pacman::setPacmanPos(Global& world) {
	Cell* nextStep;
	if ((isCollecting && !coinPath.empty()) ) {
		nextStep = coinPath.back();
		world.maze[row][col] = SPACE;
		row = nextStep->getRow();
		col = nextStep->getCol();
		coinPath.pop_back();
	}
	else if ((isChasing && !coinPath.empty())) {
		nextStep = coinPath.back();
		world.maze[row][col] = SPACE;
		row = nextStep->getRow();
		col = nextStep->getCol();
		coinPath.pop_back();
	}
	else if (isRunning && !coinPath.empty()) {
		nextStep = coinPath.back();
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
			else if(tmpCol == col) {
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
		

		coinPath.pop_back();
	}
	world.maze[row][col] = PAC;
}

void Pacman::checkCoin(Global& world)
{
	for (int i = 0; i < global::COINSAMAOUNT;i++) {
		if (world.coins[i].beenFound == false && world.coins[i].posRow == row && world.coins[i].posCol == col) {
			world.coins[i].beenFound = true;
			setCoin();
		}
	}
	
}

bool Pacman::BfsNeighbor(Global& world, int row, int col, Cell* pcurrent, vector<Cell*>& grays) {
	Cell* pneighbor;

	if (world.maze[row][col] == GRAY){
		while (pcurrent->getParent()) {
			coinPath.push_back(pcurrent);
			pcurrent = pcurrent->getParent();
		}
		return true;
	}
	else {
		pneighbor = new Cell(row, col, pcurrent); // and pcurrent as parent
		grays.push_back(pneighbor);
	}
	return false;
}

void Pacman::BfsChase(Global& world)
{
	Cell* pcurrent;
	int r, c;
	vector<Cell*> grays;
	bool sol = false;

	while (!coinPath.empty()) {
		coinPath.pop_back();
	}
	pcurrent = new Cell(row, col);
	grays.push_back(pcurrent);
	while (!sol) {
		if (grays.empty())
		{
			return;
		}
		else // grays is not empty
		{
			pcurrent = *grays.begin();
			// remove pcurrent from grays and paint it black
			grays.erase(grays.begin());
			r = pcurrent->getRow();
			c = pcurrent->getCol();
			// add non-visited neighbors
			// up
			if (world.maze[r + 1][c] == SPACE || world.maze[r + 1][c] == GRAY || world.maze[r + 1][c] == COINS)
				sol = BfsNeighbor(world, r + 1, c, pcurrent, grays);
			// down
			if (!sol && (world.maze[r - 1][c] == SPACE || world.maze[r - 1][c] == GRAY || world.maze[r + 1][c] == COINS))
				sol = BfsNeighbor(world, r - 1, c, pcurrent, grays);
			// left
			if (!sol && (world.maze[r][c - 1] == SPACE || world.maze[r][c - 1] == GRAY || world.maze[r + 1][c] == COINS))
				sol = BfsNeighbor(world, r, c - 1, pcurrent, grays);
			// right
			if (!sol && (world.maze[r][c + 1] == SPACE || world.maze[r][c + 1] == GRAY || world.maze[r + 1][c] == COINS))
				sol = BfsNeighbor(world, r, c + 1, pcurrent, grays);
		}
	}
	
}


