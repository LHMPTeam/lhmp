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

// CLog states - control client log flow
enum LOGSTATES {
	LOG_OFF,		// nothing is being logged
	LOG_NORMAL,		// loggin non-frequent messages (network ones, a few CGame functions) 
	LOG_ALL			// loggin all - callbacks, hooks (this might take a lot of HDD flow and cause FPS drop)
};

class CLog
{
	// logState is defined by LOGSTATES enum, default is LOG_NORMAL
	unsigned char logState;

	// if isDebug, then print log messages in client's chat
	bool isDebug;
public:
	CLog();
	// Insert a new @logmessage into log system
	// if @priority is BIGGER than logState, message is dismissed
	void AddLog(char* logmessage, unsigned char priority = LOG_NORMAL);
	// Set logState
	void SetLogState(unsigned char);
	// Get logState
	unsigned char GetLogState();
	// Set debug state
	void SetDebugState(bool);
	// Get debug state
	bool GetDebugState();
};
#endif