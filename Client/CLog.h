#ifndef CLOG_H
#define CLOG_H
#include <iostream>
#include <stdio.h>
#include <string.h>
class CLog
{
	bool isDebug;
public:
	CLog();
	//~CLog();
	void AddLog(char*);
	//void AddLog(char []);
	//void AddLog(std::string);
	void SetDebugState(bool);
	bool GetDebugState();
};
#endif