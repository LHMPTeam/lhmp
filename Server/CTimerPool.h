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