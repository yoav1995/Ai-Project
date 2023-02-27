#pragma once
#include "CollectCoins.hpp"
#include "ChaseGhosts.hpp"
#include "RunFromGhosts.hpp"



CollectCoins::CollectCoins()
{
}

CollectCoins::~CollectCoins()
{
}

void CollectCoins::MakeTransition(Pacman* p, Global& world)
{ 
	p->getCurrentState()->OnStateExit(p, world); // set the relevant bool flags to false
	p->setGhostDist(world);
	if (p->getDistFromGhost() < 10) {
		p->setCurrentState(new RunFromGhosts());
		p->setTimer(0);
	}
	else if (p->getTimer() >= p->getTimerAmount() *3) {
		p->setCurrentState(new ChaseGhosts());
		p->setTimer(0);
	}
	p->getCurrentState()->OnStateEnter(p,world);
}

void CollectCoins::OnStateEnter(Pacman *p, Global& world)
{
	p->setIsCollecting(true);
	p->setIsChasing(false);
	p->setIsRunning(false);
}

void CollectCoins::OnStateExit(Pacman* p, Global& world)
{
	p->setIsCollecting(false);
}
