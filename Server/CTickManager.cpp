#include "CCore.h"
#include "CTickManager.h"
#include <stdio.h>
extern CCore *g_CCore;

#include "../shared/linux.h"

CTickManager::CTickManager()
{
	lastSecond = timeGetTime();
	lastRunTickCount = timeGetTime();
	tickCount = 0;

}
void	CTickManager::Pulse()
{
	tickCount++;
	DWORD	actualTics = timeGetTime();
	if((actualTics - lastSecond) >= 1000)
	{
		//printf("TicksCount: %d \n",tickCount);
		tickCount = 0;
		lastSecond = actualTics;
		g_CCore->GetScripts()->onServerTickSecond(tickCount);
		g_CCore->OnSecondElapsed();
	}
	else
	{
		g_CCore->GetScripts()->onServerTick(tickCount);
	}
	DWORD ticksSinceLastRun = actualTics - lastRunTickCount;
	if(SERVER_TICK_SLEEP >= ticksSinceLastRun)
	{
		//printf("%d\n",ticksSinceLastRun);
		//printf("%d \n",(SERVER_TICK_SLEEP-ticksSinceLastRun));
		RakSleep((SERVER_TICK_SLEEP-ticksSinceLastRun));
	}
	lastRunTickCount = actualTics;
}

unsigned int CTickManager::GetLastTickCount()
{
	return this->lastRunTickCount;
}


unsigned int CTickManager::GetTime()
{
	return timeGetTime();
}