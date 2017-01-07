#define _WINSOCKAPI_
#include <windows.h>
#include "Core.h"

void Start()
{
	Core* core = new Core();
	core->Init();
}

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		Start();
		break;
	case DLL_PROCESS_DETACH:
		ExitProcess(0);
		break;
	}
	return TRUE;
}