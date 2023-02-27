#pragma once
#include "State.hpp"


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

