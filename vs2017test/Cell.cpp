#pragma once
#include "Cell.h"
#include <math.h>
#include <iostream>

Cell::Cell()
{
}

Cell::Cell(int r, int c)
{
	row = r;
	col = c;
	parent = nullptr; 
	g = 0;
	h = 0;
	f = 0;
	dir = 0;
}

Cell::Cell(int r, int c, Cell* p,Cell* targ,int dir)
{
	row = r;
	col = c;
	parent = p; // has no parent
	this->setG(targ);
	this->dir = dir;
}

Cell::Cell(int r, int c, Cell* p, int dir)
{
	row = r;
	col = c;
	parent = p; // has no parent
	this->computeG();
	this->dir = dir;
}

Cell::~Cell()
{
}

Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	col = c;
	parent = p;
}


void Cell::setH(int color)
{
	
	h = color;
}

void Cell::setG(Cell* targ)
{ 
	g = sqrt(pow(targ->getCol() - this->col, 2) + pow(targ->getRow() - this->row, 2));
	
}


void Cell::setF()
{
	f = parent->getF() + h + g;
}

bool Cell::operator==(const Cell& other)
{
	if (row == other.row && col == other.col)
		return true;
	else
		return false;
}

void Cell::computeH(int Trow, int Tcol)
{
	h = sqrt(pow(row - Trow, 2) + pow(col - Tcol, 2));
}
void Cell::computeG()
{
	g = parent->getG() + 1;
}
void Cell::computeF()
{
	f = h + g;
}