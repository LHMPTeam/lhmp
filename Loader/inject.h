#ifndef INC_INJECT
#define INC_INJECT

#include <windows.h>
#include <iostream>

HMODULE InjectDLL(DWORD ProcessID, char* dllName);

#endif