#include "CTimer.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/time.h>
extern unsigned long timeGetTime();
#define Sleep usleep
#endif

CTimer::CTimer(SQVM* script, char* fce, int interval, int repeat,int param)
{
	this->interval = interval;
	this->script = script;
	this->repeat = repeat;
	this->lastRun = timeGetTime();
	this->param = param;
	sprintf(this->function, fce);
}

CTimer::~CTimer()
{

}

bool CTimer::Tick()
{
	if (repeat == 0) return false;
	unsigned int time = timeGetTime();
	if ((time - this->lastRun) >= (unsigned int)interval)
	{
		this->lastRun = time;

		// Get the stack top
		int iTop = sq_gettop(this->script);

		// Push the root table onto the stack
		sq_pushroottable(this->script);

		// Push the function name onto the stack
		sq_pushstring(this->script, this->function, -1);

		if (SQ_SUCCEEDED(sq_get(this->script, -2))) {
			// Push the root table onto the stack
			sq_pushroottable(this->script);

			sq_pushinteger(this->script,param);

			sq_call(this->script,1, true, true);

		}

		// Restore the stack top
		sq_settop(this->script, iTop);
		if (repeat != -1)
			repeat--;
	}
	return true;
}
