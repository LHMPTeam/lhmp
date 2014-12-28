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