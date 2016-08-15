// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef CCONSOLE_H
#define CCONSOLE_H
#include <stdio.h>
class CConsole
{
public:
	// setup console
	void Init();
	// function which checks console input
	void Tick();
};
#endif