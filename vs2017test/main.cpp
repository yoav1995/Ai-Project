#pragma once
#include <cstdio>
#include "glut.hpp"
#include "CompareCell.hpp"
#include "CompareCellF.hpp"
#include "Globals.hpp"
#include <queue>
#include <vector>
#include "Pacman.hpp"
#include "Ghost.hpp"

using namespace global;
using namespace std;

bool lostOrWOn = false;

 Cell* startCell;
 Cell* targetCell;

int dir;
bool solutionFound;
int roomsOrder[global::ROOMAMOUNT][2] = { 0 }; 

int Srow, Scol;

Global world;

Ghost ghosts[3];

Pacman* pacman = new Pacman(world);
Cell* pc;



void DrawRoom(int i) {
	for (int j = world.rooms[i].startCol; j < world.rooms[i].startCol+ world.rooms[i].width; j++) {
		for (int k = world.rooms[i].startRow; k < world.rooms[i].startRow + world.rooms[i].height; k++) {
			world.maze[k][j] = global::SPACE;
		}
	}
}

bool checkRoomAvailable(int row,int col,int width,int height) {
	for (int i = row - 1;i <= row + height + 1;i++) {
		for (int j = col - 1; j <= col + width + 1;j++) {
			if (world.maze[i][j] == global::SPACE)
				return false;
		}
	}
	return true;
}

void createRooms() {
	cout << "Creating Rooms...\n";

	for (int i = 0;i < global::MSZ;i++) {
		for (int j = 0;j < global::MSZ;j++) {
			world.maze[i][j] = global::WALL;
		}
	}
	const int maxRoomSize = global::MSZ * global::MSZ/ global::ROOMAMOUNT;
	for (int i = 0;i < global::ROOMAMOUNT;) {
		const int row = rand() % global::MSZ;
		const int col = rand() % global::MSZ;
		const int width = rand() % (maxRoomSize/2 - 2) + 10;
		int height = rand() % (maxRoomSize / 2 - 2) + 10;
		if (width * height < maxRoomSize && row + height < global::MSZ && col + width < global::MSZ && checkRoomAvailable(row, col, width, height)) {
			world.rooms[i].startRow = row;
			world.rooms[i].startCol = col;
			world.rooms[i].height = height;
			world.rooms[i].width = width;
			world.rooms[i].midRow = row + (height / 2);
			world.rooms[i].midCol = col + (width / 2);
			DrawRoom(i);
			cout << "room " << i + 1 << " has been created.\n";
			i++;
		}
	}
}

void DrawMaze() {

	for (int i = 0;i < global::MSZ;i++) {
		for (int j = 0;j < global::MSZ;j++) {
			//set color
			switch (world.maze[i][j]) {
			case global::WALL:
				glColor3d(0.65, 0.2, 0.2);
				break;
			case global::SPACE:
				glColor3d(1, 1, 1);
				break;
			case global::START:
				glColor3d(0.1, 0.1,1);
				break;
			case global::TARG:
				glColor3d(1, 0, 0);
				break;
			case global::BLACK:
				glColor3d(0, 0, 0);
				break;
			case global::GRAY:
				glColor3d(0.4, 0.2, 0.2);
				break;
			case global::PAC:
				glColor3d(0.9, 0.8, 0);
				break;
			case global::COINS:
				glColor3d(0, 0.1, 0.9);
				break;
			case global::PATH:
				glColor3d(0.7, 0, 0.7);
				break;
			
			}
			glBegin(GL_POLYGON);
			glVertex2d(j, i); // i sorot j amodot ( i = y j = x )
			glVertex2d(j, i+1);
			glVertex2d(j+1, i+1);
			glVertex2d(j+1, i);
			glEnd();

		}
	}
}


double distant(int x1, int y1, int x2, int y2) {
	return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

void roomOrder() {
	double roomsDis[global::ROOMAMOUNT][2];
	for (int i = 0;i < global::ROOMAMOUNT;i++) {
		roomsDis[i][0] = 100000;
		roomsDis[i][1] = 100000;
	}
	for (int i = 0;i < global::ROOMAMOUNT;i++) {
		for (int j = 0;j < global::ROOMAMOUNT;j++) {
			if (j != i) {
				double check = distant(world.rooms[j].midRow, world.rooms[i].midRow, world.rooms[j].midCol, world.rooms[i].midCol);
				if (roomsDis[i][0] > check || roomsDis[i][1] > check) {
					if (check < roomsDis[i][1]) {
						if (roomsDis[i][1] < roomsDis[i][0]) {
							roomsDis[i][1] = roomsDis[i][0];
							roomsOrder[i][1] = roomsOrder[i][0];
						}
						roomsDis[i][1] = check;
						roomsOrder[i][1] = j;
					}
					else {
						if (roomsDis[i][1] > roomsDis[i][0]) {
							roomsDis[i][0] = roomsDis[i][1];
							roomsOrder[i][0] = roomsOrder[i][1];
						}
						roomsDis[i][0] = check;
						roomsOrder[i][0] = j;
					}

				}
			}
		}
	}
}


void UpdatePQ(Cell* neighbor, priority_queue <Cell*, vector<Cell*>, CompareCellF>& pq)
{
	vector <Cell*> tmp;
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
		cout << "Error! PQ is empty in UpdatePQ\n";
		exit(1);
	}
	pq.pop();
	pq.push(neighbor);
	while (!tmp.empty())
	{
		pq.push(tmp.back());
		tmp.pop_back();
	}
}


void RestorePath(Cell* pc) {

	while (pc->getParent() != nullptr) {
		world.maze[pc->getRow()][pc->getCol()] = global::SPACE;
		pc = pc->getParent();
	}
	world.maze[pc->getRow()][pc->getCol()] = global::SPACE;
}

bool checkneighbor(int row, int col, Cell* pCorr, priority_queue<Cell*, vector<Cell*>, CompareCellF>& pqA, vector<Cell>& black, vector<Cell>& grays, int dir, int type) {
	vector<Cell>::iterator itGray, itBlack;
	Cell* neighbor = new Cell(row, col, pCorr, targetCell, dir);
	if (col <= 0 || col >= global::MSZ)
		return false;
	if (neighbor->getG() == 0) {
		if (type == 0)
			RestorePath(pCorr);
		else {
			vector<Cell> tmp;
			while (pCorr) {
				tmp.push_back(*pCorr);
				pCorr = pCorr->getParent();
			}
		}
		return true;
	}
	if (type == 0) {
		if (world.maze[row][col] == global::SPACE)
			neighbor->setH(1);
		else
			neighbor->setH(9);
		neighbor->setF();

	}
	else {
		neighbor->computeH(targetCell->getRow(), targetCell->getCol());
	}
	itGray = find(grays.begin(), grays.end(), *neighbor);
	itBlack = find(black.begin(), black.end(), *neighbor);

	if (itGray == grays.end() && itBlack == black.end()) {
		pqA.push(neighbor);
		grays.push_back(*neighbor);
	}
	else if (itGray != grays.end()) {
		if (neighbor->getF() < itGray->getF()) {
			*itGray = *neighbor;
			UpdatePQ(neighbor, pqA);
		}
	}

	return false;
}


void runCreatePathAStar(priority_queue<Cell*, vector<Cell*>, CompareCellF>& pqA, vector<Cell>& grays)
{
	// create vector Gray and Black
	vector<Cell> black;
	vector<Cell>::iterator itGray;
	Cell* pCurrent;
	solutionFound = false;

	while (!solutionFound)
	{
		if (pqA.empty())
		{
			return;
		}
		pCurrent = pqA.top();
		// paint pCurrent black and extract it from pq
		pqA.pop();
		itGray = find(grays.begin(), grays.end(), *pCurrent);
		if (itGray == grays.end()) // pCurrent wasn't found
		{
			cout << "ERROR: pCurrent wasn't found\n";
			return;
		}
		grays.erase(itGray);
		black.push_back(*pCurrent);
		const int r = pCurrent->getRow();
		const int c = pCurrent->getCol();

		solutionFound = checkneighbor(r, c - 1, pCurrent, pqA, black, grays, 1, 0);
		solutionFound = checkneighbor(r - 1, c, pCurrent, pqA, black, grays, 3, 0);
		solutionFound = checkneighbor(r, c + 1, pCurrent, pqA, black, grays, 2, 0);
		solutionFound = checkneighbor(r + 1, c, pCurrent, pqA, black, grays, 4, 0);
	}
}

void CreatePaths() {

	cout << "Starting to create paths..\n";

	roomOrder();
	for (int i = 0; i < global::ROOMAMOUNT; i++) {
		vector<Cell> grays;
		priority_queue<Cell*, vector<Cell*>, CompareCellF> pqA;
		while (!pqA.empty())
			pqA.pop();
		grays.clear();
		startCell = new Cell(world.rooms[i].midRow, world.rooms[i].midCol) ;
		targetCell = new Cell(world.rooms[roomsOrder[i][0]].midRow, world.rooms[roomsOrder[i][0]].midCol);
		grays.push_back(*startCell);
		pqA.push(startCell);
		world.maze[startCell->getRow()][startCell->getCol()] = global::SPACE;
		world.maze[targetCell->getRow()][targetCell->getCol()] = global::TARG;
		runCreatePathAStar(pqA, grays);
		world.maze[targetCell->getRow()][targetCell->getCol()] = global::SPACE;
		targetCell = new Cell(world.rooms[roomsOrder[i][1]].midRow, world.rooms[roomsOrder[i][1]].midCol);
		world.maze[startCell->getRow()][startCell->getCol()] = global::SPACE;
		world.maze[targetCell->getRow()][targetCell->getCol()] = global::TARG;
		runCreatePathAStar(pqA, grays);
		cout << "Paths for room " << i + 1 << " has been drawn.\n";
	}
}


void init()
{
	srand(time(0));
	createRooms();
	CreatePaths();
	for(int i = 0;i< global::MSZ;i++)
		for (int j = 0;j < global::MSZ;j++) {
			if (world.maze[i][j] == global::START || world.maze[i][j] == global::TARG)
				world.maze[i][j] = global::SPACE;
		}
	int i = startCell->getRow();
	int j = startCell->getCol();
	world.maze[i][j] = global::PAC;
	pacman->setRow(i);
	pacman->setCol(j);
	int count = 0;

	for (i = 0;i < ROOMAMOUNT;i++) {
		world.maze[world.rooms[i].startRow + world.rooms[i].height / 3][world.rooms[i].startCol + world.rooms[i].width / 3] = COINS;
		world.maze[world.rooms[i].startRow + 2*world.rooms[i].height / 3][world.rooms[i].startCol + 2 * world.rooms[i].width / 3] = COINS;
		world.coins[2 * i].posRow = world.rooms[i].startRow + world.rooms[i].height / 3;
		world.coins[2 * i].posCol = world.rooms[i].startCol + world.rooms[i].width / 3;
		world.coins[(2 * i) + 1].posRow = world.rooms[i].startRow + 2 * world.rooms[i].height / 3;
		world.coins[(2 * i) + 1].posCol = world.rooms[i].startCol + 2 * world.rooms[i].width / 3;
	}

	for (int i = 0; i < 3;i++)
	{
		ghosts[i].setRow(world.rooms[i].midRow);
		ghosts[i].setCol(world.rooms[i].midCol);
		world.maze[ghosts[i].getRow()][ghosts[i].getCol()] = global::GRAY;
	}

	pacman->setGhostDist(world);

	glClearColor(0.65, 0.2, 0.2, 0); 
	glOrtho(0, global::MSZ, 0, global::MSZ ,-1, 1);
}
	
	

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	if (!lostOrWOn) {
		if ((pacman->getDistFromGhost() < 10 && pacman->getIsCollecting()) || pacman->getTimer() >= pacman->getTimerAmount() || (pacman->getDistFromGhost() > 10 && pacman->getIsRunning())) {
			pacman->getCurrentState()->MakeTransition(pacman, world);
			pacman->setTimerAmount(5);
		}
			

		ghosts[0].setPacRow(pacman->getRow());
		ghosts[0].setPacRow(pacman->getCol());

		ghosts[1].setPacRow(pacman->getRow());
		ghosts[1].setPacRow(pacman->getCol());

		ghosts[2].setPacRow(pacman->getRow());
		ghosts[2].setPacRow(pacman->getCol());

		if(ghosts[0].getEaten() == false)
			ghosts[0].runGhostSearch(world);
		if (ghosts[1].getEaten() == false)
			ghosts[1].runGhostSearch(world);
		if (ghosts[2].getEaten() == false)
			ghosts[2].runGhostSearch(world);

		if (pacman->getIsCollecting()) {
			pacman->runCoinsSearch(world, global::COINS);
		}
		else if (pacman->getIsChasing()) {
			pacman->BfsChase(world);
		}
		else if (pacman->getIsRunning()) {
			pacman->runCoinsSearch(world, global::GRAY);
		}
		if (pacman->getIsChasing()) {
			ghosts[0].checkEaten(world, pacman->getRow(), pacman->getCol());
			ghosts[1].checkEaten(world, pacman->getRow(), pacman->getCol());
			ghosts[2].checkEaten(world, pacman->getRow(), pacman->getCol());
		}
		if (pacman->getIsRunning()) {
			for (int i = 0;i < 3;i++) {
				if (pacman->getRow() == ghosts[i].getRow() && pacman->getCol() == ghosts[i].getCol() && !ghosts[i].getEaten()) {
					cout << "Pacman Lost\n";
					lostOrWOn = true;
				}
			}
		}
		
		if (pacman->getCoin() == COINSAMAOUNT) {
			cout << "Pacman Won\n";
			lostOrWOn = true;
		}
		else if (ghosts[0].getEaten() && ghosts[1].getEaten() && ghosts[2].getEaten()) {
			cout << "Pacman Won\n";
			lostOrWOn = true;
		}
	
	}
	
	DrawMaze();
	glutSwapBuffers();
}

void idle()
{
	if (!lostOrWOn) {
		pacman->setPacmanPos(world);
		pacman->checkCoin(world);
		
		if (ghosts[0].getEaten() == false)
			ghosts[0].setGhostPos(world, pacman->getIsChasing());
		if (ghosts[1].getEaten() == false)
			ghosts[1].setGhostPos(world, pacman->getIsChasing());
		if (ghosts[2].getEaten() == false)
			ghosts[2].setGhostPos(world, pacman->getIsChasing());
		pacman->addTimer();
		pacman->setGhostDist(world);
		
	}
	
	
	glutPostRedisplay();
}


void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("End Project");

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	init();

	glutMainLoop();
}