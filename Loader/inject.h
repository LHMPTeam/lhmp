// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef INC_INJECT
#define INC_INJECT

#include <windows.h>
#include <iostream>

HMODULE InjectDLL(DWORD ProcessID, char* dllName);

#endif