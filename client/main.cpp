#include <stdinc.h> 

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
		DisableThreadLibraryCalls(hinstDLL);

		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&Start, 0, 0, 0);
		break;
	case DLL_PROCESS_DETACH:
		ExitProcess(0);
		break;
	}
	return TRUE;
}