#include "CLog.h"
#include "CCore.h"
#include <iostream>
#include <fstream>

extern CCore *g_CCore;

CLog::CLog()
{
//#ifdef _DEBUG
	//this->SetDebugState(true);
//#else
	this->SetDebugState(false);
//#endif
}
void CLog::AddLog(char* str)
{
	if (this->isDebug == 1)
	{
		char buf[250];
		sprintf_s(buf, "#aaaaaa%s", str);
		g_CCore->GetChat()->AddMessage(buf);
		std::fstream zapis;
		zapis.open("lhmp/log.txt",std::ios::app);
		zapis << str << std::endl;
		zapis.close();
	}

}

/*void CLog::AddLog(std::string str)
{
	this->AddLog(str.c_str());
}*/

void CLog::SetDebugState(bool b)
{
	this->isDebug = b;
}
bool CLog::GetDebugState()
{
	return this->isDebug;
}