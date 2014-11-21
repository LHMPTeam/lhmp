// main.cpp

#include <windows.h>
#include <iostream>
#include "inject.h"

const char* EXE_NAME = "Game.exe"; // target executable
const char* DLL_NAME = "DirectXHook.dll"; // dll to inject

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	char path[MAX_PATH];
	char exename[MAX_PATH];
	char dllname[MAX_PATH];

	// aquire full path to exe:
	GetModuleFileNameA(0, path, MAX_PATH);

	// find the position of the last backslash and delete whatever follows
	// (eg C:\Games\loader.exe becomes C:\Games\)
	int pos = 0;
	for (int k = 0; k < strlen(path); k++) {
		if (path[k] == '\\') {
			pos = k;
		}
	}
	path[pos+1] = 0; // null-terminate it for strcat

	// build path to target
	strcpy_s(exename, path);
	strcat_s(exename, EXE_NAME);

	// build path to dll
	strcpy_s(dllname, path);
	strcat_s(dllname, DLL_NAME);

	// launch program:
	STARTUPINFOA siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	if (!CreateProcessA(NULL,
					exename, 0, 0, false,
					CREATE_SUSPENDED, 0, 0,
			&siStartupInfo, &piProcessInfo)) {
		MessageBoxA(NULL, exename, "Error", MB_OK); 
	}

	// get the process id for injection
	DWORD pId = piProcessInfo.dwProcessId;

	// Inject the dll
	if (!InjectDLL(pId, dllname)) {
		MessageBoxA(NULL, "Injection failed", "Error", MB_OK);		
	}

	ResumeThread(piProcessInfo.hThread);

	return 0;
}