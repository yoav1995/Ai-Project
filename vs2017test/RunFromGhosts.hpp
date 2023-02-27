#pragma once
#include "State.hpp"
class RunFromGhosts: public State
{
public:
	RunFromGhosts();
	~RunFromGhosts();

	void MakeTransition(Pacman* p,Global& world);
	void OnStateEnter(Pacman* p, Global& world);
	void OnStateExit(Pacman* p, Global& world);
};

