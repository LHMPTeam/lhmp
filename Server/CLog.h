/**
Lost Heaven Multiplayer

Purpose: log class

@author Romop5
@version 1.0 1/9/14
*/

#ifndef LOG_C
#define LOG_C

enum LogTypes
{
	LOG_DISABLED = 0x0,
	LOG_NORMAL = 0x1,			// use to log player text messages, connections and everything you want to print to console
	DEBUG_NORMAL = 0x2,
	DEBUG_LOWPRIORITY = 0x4		// use for something which is too frequent - server ticks, updates etc.
};

class CLog
{
private:
	// log mode - log saves only log messages which are allowed in this control - default is LOG_NORMAL
	unsigned int uiLogMode;
	// set if DEBUG_NORMAL messages should be print in server console - default value is false
	bool bPrintDebug;
	// dumps message on HDD, into 'logs' folder, file name format YYYY:MM:DD, and adds 'debug' if true
	void WriteToDisc(char*, bool);
public:
	CLog();
	void AddLog(int, char*);
	void AddNormalLog(char*, ...);
	void AddDebug(char*, ...);
};
#endif
