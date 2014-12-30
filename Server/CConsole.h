/**
Lost Heaven Multiplayer

Purpose: server console for server owner

@author Romop5
@version 1.0 1/9/14
*/

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