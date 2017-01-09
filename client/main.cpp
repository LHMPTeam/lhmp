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
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		Start();
		break;
	case DLL_PROCESS_DETACH:
		ExitProcess(0);
		break;
	}
	return TRUE;
}