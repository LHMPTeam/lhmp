#include "CCore.h"
#include "CTickManager.h"
#include <stdio.h>
extern CCore *g_CCore;


#ifndef _WIN32
#include <sys/time.h>
unsigned long timeGetTime()
{
/*struct timeval now;
gettimeofday(&now, NULL);
return now.tv_usec/1000;*/

	timespec timespec_time;
	__uint32_t theTick = 0;

	clock_gettime(CLOCK_REALTIME, &timespec_time);

	theTick = timespec_time.tv_nsec / 1000000;
	theTick += timespec_time.tv_sec * 1000;
	return theTick;
}
#define Sleep usleep
#endif
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