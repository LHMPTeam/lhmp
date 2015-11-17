/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose pool of timers
@author Romop5
******************************************************************************/

#include "CTimer.h"
class CTimerPool
{
private:
	CTimer* pool[100];
public:
	CTimerPool();
	~CTimerPool();
	void	Tick();
	// script handle,script function, run interval, repeat count(-1 = for ever)
	int		New(SQVM*,char*,int,int,int);
	void	Delete(int);
};