#include "squirrelheads.h"

void printfunc(HSQUIRRELVM v, const SQChar *s, ...)
{
	va_list arglist;
	va_start(arglist, s);
	scvprintf(s, arglist);
	va_end(arglist);
	std::cout << std::endl;
}