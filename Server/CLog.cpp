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
	char endBuff[500] = "";
	int start = 0;
	int len = strlen(format);
	int count = 0;
	for (int i = 0; i < len; i++)
	{
		if (format[i] == '%')
			count++;
	}
	if (count > 0)
	{
		va_list vl;
		va_start(vl, format);
		for (int i = 0; i < len; i++)
		{
			if (format[i] == '%' && len - 1 != i)
			{
				switch (format[i + 1])
				{
				case '%':
					sprintf(endBuff, "%s%%", endBuff);
					break;
				case 'i':
				case 'd':
					sprintf(endBuff, "%s%d", endBuff, va_arg(vl, int));
					break;
				case 'f':
					sprintf(endBuff, "%s%f", endBuff, va_arg(vl, double));
					break;
				case 'u':
					sprintf(endBuff, "%s%u", endBuff, va_arg(vl, unsigned int));
					break;
				case 'x':
				case 'X':
					sprintf(endBuff, "%s%X", endBuff, va_arg(vl, unsigned int));
					break;
				case 'p':
					sprintf(endBuff, "%s%p", endBuff, va_arg(vl, unsigned int));
					break;
				case 's':
					sprintf(endBuff, "%s%s", endBuff, va_arg(vl, char*));
					break;
				default:
					break;
				}
				i++;
			}
			else {
				sprintf(endBuff, "%s%c", endBuff, format[i]);
			}
		}
		va_end(vl);
	}
	else {
		sprintf(endBuff, "%s", format);
	}

	// let's see the result
	//this->AddLog(1,endBuff);
	printf("%s\n", endBuff);
	this->WriteToDisc(endBuff, false);

}

void CLog::AddDebug(char* format, ...)
{
	char endBuff[500] = "";
	int start = 0;
	int len = strlen(format);
	int count = 0;
	for (int i = 0; i < len; i++)
	{
		if (format[i] == '%')
			count++;
	}
	if (count > 0)
	{
		va_list vl;
		va_start(vl, format);
		for (int i = 0; i < len; i++)
		{
			if (format[i] == '%' && len - 1 != i)
			{
				switch (format[i + 1])
				{
				case '%':
					sprintf(endBuff, "%s%%", endBuff);
					break;
				case 'i':
				case 'd':
					sprintf(endBuff, "%s%d", endBuff, va_arg(vl, int));
					break;
				case 'f':
					sprintf(endBuff, "%s%f", endBuff, va_arg(vl, double));
					break;
				case 'u':
					sprintf(endBuff, "%s%u", endBuff, va_arg(vl, unsigned int));
					break;
				case 'x':
				case 'X':
					sprintf(endBuff, "%s%X", endBuff, va_arg(vl, unsigned int));
					break;
				case 'p':
					sprintf(endBuff, "%s%p", endBuff, va_arg(vl, unsigned int));
					break;
				case 's':
					sprintf(endBuff, "%s%s", endBuff, va_arg(vl, char*));
					break;
				default:
					break;
				}
				i++;
			}
			else {
				sprintf(endBuff, "%s%c", endBuff, format[i]);
			}
		}
		va_end(vl);
	}
	else {
		sprintf(endBuff, "%s", format);
	}

	// let's see the result
	//this->AddLog(1,endBuff);
	if(this->bPrintDebug)
		printf("%s\n", endBuff);
	this->WriteToDisc(endBuff, true);

}
