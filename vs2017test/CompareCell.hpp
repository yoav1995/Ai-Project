#pragma once
#include "Cell.hpp"

class CompareCell
{
public:
	CompareCell();
	~CompareCell();
	bool operator()(Cell* c1, Cell* c2) {
		return c1->getH() > c2->getH();
		
	}

};

