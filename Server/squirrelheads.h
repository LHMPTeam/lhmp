// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include <stdarg.h>
#include <stdio.h>
#include <squirrel.h>
#include <sqstdio.h>
#include <sqstdaux.h>


#include <sqstdstring.h>
#include <sqstdmath.h>
#include <sqstdsystem.h>

#ifdef SQUNICODE
#define scvprintf vwprintf
#else
#define scvprintf vprintf
#endif
void printfunc(HSQUIRRELVM v, const SQChar *s, ...);


#include "sq_functions.h"



