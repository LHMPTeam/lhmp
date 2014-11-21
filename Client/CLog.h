/**
	Lost Heaven Multiplayer

	Purpose: logging class -> into chat or file

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code, AddLog with unlimited params
*/
#ifndef __CLOG_H
#define __CLOG_H
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