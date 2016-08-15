// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

// Contains Linux utils
#pragma once
#ifndef _WIN32
typedef unsigned char BYTE;
typedef unsigned char byte;
typedef unsigned long DWORD;

#define sprintf_s sprintf

#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
static void Sleep(unsigned int ms)
{
	//usleep(ms);
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 1000000 * ms;

	nanosleep(&tim, &tim2);
}

static unsigned int GetTickCount()
{
	struct timespec ts;
	unsigned theTick = 0U;
	clock_gettime(CLOCK_REALTIME, &ts);
	theTick = ts.tv_nsec / 1000000;
	theTick += ts.tv_sec * 1000;
	return theTick;
}

static unsigned int timeGetTime()
{
	return GetTickCount();
}

static void strcpy_s(const char* dest, unsigned int length, const char* source)
{
	strcpy((char*)dest, source);
}
#endif
