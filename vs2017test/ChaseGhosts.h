#pragma once
#include "State.h"


class ChaseGhosts :
	public State
{
public:
	ChaseGhosts();
	~ChaseGhosts();

	void MakeTransition(Pacman* p, Global& world);
	void OnStateEnter(Pacman* p, Global& world);
	void OnStateExit(Pacman* p, Global& world);
};

