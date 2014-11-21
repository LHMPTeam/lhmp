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
public:
	CTimer(SQVM*,char*,int,int);
	~CTimer();
	bool Tick();
};