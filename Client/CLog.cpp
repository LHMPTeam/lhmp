// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CLog.h"
#include "CCore.h"
#include <iostream>
#include <fstream>

extern CCore *g_CCore;

CLog::CLog()
{
	logState = LOG_NORMAL;
//#ifdef _DEBUG
	//this->SetDebugState(true);
//#else
	this->SetDebugState(false);
//#endif
	this->AddLog("------------------------------------------");
}
void CLog::AddLog(char* str, unsigned char priority)
{
	// if message has enough priority 
	if (priority <= this->logState)
	{
		// append log message into logfile
		std::fstream logFile;
		logFile.open("lhmp/log.txt", std::ios::app);
		if (logFile.is_open())
		{
			logFile << str << std::endl;
			logFile.close();
		}
		// if isDebug, print message in chat
		if (this->isDebug == 1)
		{
			char buf[250];
			sprintf_s(buf, "#aaaaaa%s", str);
			g_CCore->GetChat()->AddMessage(buf);
		}
	}
}

void  CLog::SetLogState(unsigned char state)
{
	this->logState = state;
}
unsigned char  CLog::GetLogState()
{
	return this->logState;
}

void CLog::SetDebugState(bool b)
{
	this->isDebug = b;
}
bool CLog::GetDebugState()
{
	return this->isDebug;
}