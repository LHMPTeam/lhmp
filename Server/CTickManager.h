#ifndef CTICKMANAGER_H
#define CTICKMANAGER_H
//typedef unsigned char byte;

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
public:
	CTickManager();
	void	SetTickCount(unsigned short);
	void	Pulse();
	unsigned int GetLastTickCount();
	unsigned int GetTime();
};
#endif