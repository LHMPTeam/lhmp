#include <Windows.h>
#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call) {
		case DLL_PROCESS_ATTACH: {
			DisableThreadLibraryCalls(hModule);
			// Apply the hook
			while(GetModuleHandleA("d3d8.dll") == NULL)
			{
				Sleep(10);
			}
			AllocConsole();
			AttachConsole( GetCurrentProcessId() ) ;
			freopen( "CON", "w", stdout ) ;
			if(GetModuleHandleA("d3d8.dll") != NULL)
				std::cout << "working, asi";
			//MessageBoxA(NULL,"lol","wtf",MB_OK);
			//orig_LoadLibrary = (LoadLibrary_t)DetourFunction((LPBYTE) LoadLibraryA, (LPBYTE) LoadLibrary_Hook );

		}
	}
	return TRUE;
}