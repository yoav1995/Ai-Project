#pragma once
#include "ChaseGhosts.hpp"
#include "CollectCoins.hpp"
#include "RunFromGhosts.hpp"

ChaseGhosts::ChaseGhosts()
{
}


ChaseGhosts::~ChaseGhosts()
{
}

void ChaseGhosts::MakeTransition(Pacman* p , Global& world)
{
	p->getCurrentState()->OnStateExit(p,world); // set the relevant bool flags to false
	p->setGhostDist(world);
	if (p->getTimer() >= p->getTimerAmount()) {

		if (p->getDistFromGhost() < 10) {
			p->setCurrentState(new RunFromGhosts());
		}
		else
			p->setCurrentState(new CollectCoins());

		p->setTimer(0);
	}
	p->getCurrentState()->OnStateEnter(p, world);
}

void ChaseGhosts::OnStateEnter(Pacman* p, Global& world)
{
	p->setIsCollecting(false);
	p->setIsChasing(true);
	p->setIsRunning(false);
}

void ChaseGhosts::OnStateExit(Pacman* p, Global& world)
{
	p->setIsChasing(false);

}
