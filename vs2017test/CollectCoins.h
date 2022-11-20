#pragma once
#include "State.h"

class CollectCoins: 
	public State
{
public:
	CollectCoins();
	~CollectCoins();

	void MakeTransition(Pacman* p, Global& world);
	void OnStateEnter(Pacman* p, Global& world);
	void OnStateExit(Pacman* p, Global& world);
};

