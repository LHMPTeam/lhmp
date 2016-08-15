// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CCore.h"
#include "squirrelheads.h"

extern CCore* g_CCore;

void printfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
	//int len = strlen(s);
	//char* buff = new char[len * 2];

	va_list arglist;
	va_start(arglist, s);
	scvprintf(s, arglist);
	printf("\n");
	//vsprintf(buff, s, arglist);
	va_end(arglist);
	//g_CCore->GetLog()->AddNormalLog(buff, arglist);
	//delete buff;
	
}