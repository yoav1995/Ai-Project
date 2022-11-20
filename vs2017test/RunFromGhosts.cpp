#pragma once
#include "RunFromGhosts.h"
#include "CollectCoins.h"
#include "ChaseGhosts.h"

RunFromGhosts::RunFromGhosts() = default;

RunFromGhosts::~RunFromGhosts() = default;

void RunFromGhosts::MakeTransition(Pacman* p, Global& world)
{
	p->getCurrentState()->OnStateExit(p, world); // set the relevant bool flags to false
	p->setGhostDist( world);
	if (p->getDistFromGhost() > 10)
		p->setCurrentState(new CollectCoins());
	else if (p->getTimer() >= p->getTimerAmount()) {
		p->setCurrentState(new ChaseGhosts());
		p->setTimer(0);
	}
		
	p->getCurrentState()->OnStateEnter(p, world);
}

void RunFromGhosts::OnStateEnter(Pacman* p, Global& world)
{
	p->setIsRunning(true);
	p->setIsCollecting(false);
	p->setIsChasing(false);
}

void RunFromGhosts::OnStateExit(Pacman* p, Global& world)
{
	p->setIsRunning(false);
	
}
