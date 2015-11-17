/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose class for scriptable timer
@author Romop5
******************************************************************************/

#include "squirrelheads.h"
#include "RakNetTime.h"
class CTimer
{
private:
	SQVM*			script;
	int				repeat;
	char			function[255];
	unsigned int	lastRun;
	int				interval;
	int				param;
public:
	CTimer(SQVM*,char*,int,int,int);
	~CTimer();
	bool Tick();
};