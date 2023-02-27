#pragma once
#include <math.h>
#include <time.h>
#include <iostream>
#include "Globals.hpp"
#include <cstdio>


class Cell
{
private:
	int row;
	int col;
	Cell* parent;
	double h;
	double f;
	double g;
	int dir;

public:
	Cell();
	Cell(int r, int c);
	Cell(int r, int c, Cell* p,Cell* targ,int dir);
	Cell(int r, int c, Cell* p, int dir);
	Cell(int r, int c, Cell* p);
	~Cell();
	int getRow() { return row; }
	int getCol() { return col; }
	int setRow(int r) {this->row=r ; }
	int setCol(int c) { this->col = c; }
	Cell* getParent() { return parent; }
	void setParent(Cell* par) { parent = par; }
	double getH() { return h; }
	void setH(int color);
	void computeH(int Trow, int Tcol);
	void computeG();
	void computeF();
	double getG() { return g; }
	void setG(Cell* targ);
	void setF();
	double getF() { return f; }
	bool operator == (const Cell& other);
};