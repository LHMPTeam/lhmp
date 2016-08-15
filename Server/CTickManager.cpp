// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "CTickManager.h"
#include <stdio.h>
extern CCore *g_CCore;

#include "../shared/linux.h"

CTickManager::CTickManager()
{
	lastSecond = RakNet::GetTimeMS();
	lastRunTickCount = RakNet::GetTimeMS();
	tickCount = 0;
	shouldElapsed = 0;
	this->SetTickCount(30);
}
void	CTickManager::Pulse()
{
	shouldElapsed += this->milisecPerTick;
	tickCount++;
	DWORD	actualTics = RakNet::GetTimeMS();
	if((actualTics - lastSecond) >= 1000)
	{
		//printf("TicksCount: %u \n",tickCount);
		tickCount = 0;
		shouldElapsed = 0;
		lastSecond = actualTics;
		g_CCore->GetScripts()->onServerTickSecond(tickCount);
		g_CCore->OnSecondElapsed();
	}
	else
	{
		g_CCore->GetScripts()->onServerTick(tickCount);
	}

	actualTics = RakNet::GetTimeMS();
	DWORD ticksSinceLastRun = actualTics - lastSecond;
	if (ticksSinceLastRun < shouldElapsed)
	{
		DWORD sleep = (shouldElapsed - ticksSinceLastRun);
		//printf("Sleep %u (%u-%u)\n", sleep, shouldElapsed,ticksSinceLastRun);
		RakSleep(sleep);
	}
	lastRunTickCount = actualTics;
}

void	CTickManager::SetTickCount(unsigned short count)
{
	this->tickCount = count;
	this->milisecPerTick = (1000 / count);
}

unsigned int CTickManager::GetLastTickCount()
{
	return this->lastRunTickCount;
}


unsigned int CTickManager::GetTime()
{
	return RakNet::GetTimeMS();
}