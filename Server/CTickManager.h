/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose ensures ticks are called on stable frequency (1/tickcount)
		 E.g. 30 tickcount means that the pulse function is called every 33ms
@author Romop5
******************************************************************************/

#ifndef CTICKMANAGER_H
#define CTICKMANAGER_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include "../shared/linux.h"
class CTickManager
{
private:
	DWORD lastSecond;
	DWORD lastRunTickCount;
	// the count of MS that system should elapsed this second
	DWORD shouldElapsed;

	unsigned short tickCount;
	unsigned short milisecPerTick;

	// Get timestamp (provided by RakNet)
	unsigned int GetTime();
public:
	CTickManager();
	void	SetTickCount(unsigned short);
	void	Pulse();
	// Returns the count of ticks in last second (for debugging purpose)
	unsigned int GetLastTickCount();
};
#endif