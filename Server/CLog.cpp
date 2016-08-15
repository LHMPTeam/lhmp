// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CLog.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdarg.h>
#include <ctime>
#include <string.h>
#include "../shared/version.h"


#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#endif
CLog::CLog()
{
	this->bPrintDebug = true;
#ifndef _DEBUG
	this->uiLogMode = LOG_NORMAL;
#else
	this->uiLogMode = LOG_NORMAL | DEBUG_NORMAL;
#endif
}

void CLog::WriteToDisc(char* message, bool isDebug)
{
	if (isDebug && ((this->uiLogMode & DEBUG_NORMAL) == false))
		return;
#ifdef _WIN32
	CreateDirectoryEx(".", "logs", NULL);
#else
	mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif

	std::fstream writeBuff;
	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, 80, "%Y-%m-%d", timeinfo);
	char filename[100];
	//if (isDebug)
	//	sprintf(filename, "logs/%s-debug.txt", buffer);
	//else
	sprintf(filename, "logs/%s.txt", buffer);
	writeBuff.open(filename, std::ios::app);
	if (writeBuff.is_open())
	{
		writeBuff << message << std::endl;
		writeBuff.close();
	}

}

void CLog::AddLog(int mode, char* message)
{
	// TODO
	if (mode != NULL && this->uiLogMode != NULL)
	{
		if (mode == LOG_NORMAL || (mode == DEBUG_NORMAL && bPrintDebug == true))
		{
			printf("%s\n",message);
		}
		if (mode == LOG_NORMAL)
		{
			//this->WriteToDisc(message, false);
		}
		else if (mode == DEBUG_NORMAL)
		{
			//this->WriteToDisc(message, true);
		}
	}
}


void CLog::AddNormalLog(char* format, ...)
{
	char endbuff[500] = "";
	va_list args;
	va_start(args, format);
	vsprintf(endbuff, format, args);
	va_end(args);
	printf("%s\n", endbuff);
	this->WriteToDisc(endbuff, false);
}

void CLog::AddDebug(char* format, ...)
{
	char endbuff[500] = "";
	va_list args;
	va_start(args, format);
	vsprintf(endbuff, format, args);
	va_end(args);
	printf("%s\n", endbuff);
	
	if(this->bPrintDebug)
		printf("%s\n", endbuff);
	this->WriteToDisc(endbuff, true);

}
