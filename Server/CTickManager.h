#ifndef CTICKMANAGER_H
#define CTICKMANAGER_H
//typedef unsigned char byte;

#ifdef _WIN32
#include <Windows.h>
#endif

#include "../shared/linux.h"


#define SERVER_TICK	20
#define SERVER_TICK_SLEEP (1000/SERVER_TICK)
class CTickManager
{
private:
	DWORD lastSecond;
	DWORD lastRunTickCount;
	DWORD tickCount;
public:
	CTickManager();
	void	Pulse();
	unsigned int GetLastTickCount();
	unsigned int GetTime();
};
#endif