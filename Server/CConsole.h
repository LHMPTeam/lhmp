/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose command console for server-owner
@author Romop5
******************************************************************************/

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