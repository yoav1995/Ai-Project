#pragma once
#include "Pacman.h"
#include "Globals.h"
// pure virtual class (interface)
class Pacman;
class State
{
public:
	State();
	~State();

	virtual void MakeTransition(Pacman* p,Global& world) = 0;
	virtual void OnStateEnter(Pacman* p, Global& world) = 0;
	virtual void OnStateExit(Pacman* p, Global& world) = 0;
};

