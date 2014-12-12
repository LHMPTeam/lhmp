#include <Windows.h>
#include <stdio.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "resource.h"
#include "main.h"
#include "inject.h"

HINSTANCE g_hIstance = NULL;
HMODULE RemoteLoadLibrary(HANDLE,const char*);
bool CheckFileExist(char [255]);
bool IsGameRunning();
char path[MAX_PATH];
char exename[MAX_PATH];
char dllname[MAX_PATH];


int WINAPI WinMain ( HINSTANCE _hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	if(!CheckFileExist(dll_name))
	{
		MessageBox(NULL,"File 'client.dll' was not found. Reinstall LH-MP or Mafia game.","Not found",MB_OK|MB_ICONERROR);
		return 6;
	}

	if(!CheckFileExist(exe_name))
	{
		MessageBox(NULL,"File 'Game.exe' was not found. Reinstall LH-MP or Mafia game.","Not found",MB_OK|MB_ICONERROR);
		return 5;
	}

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
	strcat_s(exename, exe_name);
	strcat_s(exename," ");
	strcat_s(exename, lpCmdLine);
	

	// build path to dll
	strcpy_s(dllname, path);
	strcat_s(dllname, dll_name);

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
		char buff[200];
		DWORD errorID = GetLastError();
		if (errorID == ERROR_ELEVATION_REQUIRED)
		{
			MessageBoxA(NULL, "Game process creation has failed. Please, run as an ADMINISTRATOR !", "Error", MB_OK | MB_ICONERROR);
		}
		else {
			sprintf(buff, "CreateProcess failed. Error ID: 0x%x\n", errorID);
			MessageBoxA(NULL, buff, "Error", MB_OK | MB_ICONERROR);
		}
		return 1;
	}

	// get the process id for injection
	DWORD pId = piProcessInfo.dwProcessId;

	// Inject the dll
	if (!InjectDLL(pId, dllname)) {
		MessageBoxA(NULL, "Injection failed", "Error", MB_OK | MB_ICONERROR);
	}

	ResumeThread(piProcessInfo.hThread);

	return 0;
}


HMODULE RemoteLoadLibrary(HANDLE hProcess, const char* szLibPath)
{
    /* Called correctly? */
    if ( szLibPath == NULL )
    {
        return 0;
    }

    /* Allocate memory in the remote process for the library path */
    HANDLE hThread = 0;
    void* pLibPathRemote = NULL;
    HMODULE hKernel32 = GetModuleHandle( "Kernel32" );
    pLibPathRemote = VirtualAllocEx( hProcess, NULL, strlen ( szLibPath ) + 1, MEM_COMMIT, PAGE_READWRITE );
    
    if ( pLibPathRemote == NULL )
    {
        return 0;
    }

    /* Make sure pLibPathRemote is always freed */
    __try {

        /* Write the DLL library path to the remote allocation */
        DWORD byteswritten = 0;
        WriteProcessMemory ( hProcess, pLibPathRemote, (void*)szLibPath, strlen ( szLibPath ) + 1, &byteswritten );

        if ( byteswritten != strlen ( szLibPath ) + 1 )
        {
            return 0;
        }

        /* Start a remote thread executing LoadLibraryA exported from Kernel32. Passing the
           remotly allocated path buffer as an argument to that thread (and also to LoadLibraryA)
           will make the remote process load the DLL into it's userspace (giving the DLL full
           access to the game executable).*/
        hThread = CreateRemoteThread(   hProcess,
                                        NULL,
                                        0,
                                        reinterpret_cast < LPTHREAD_START_ROUTINE > ( GetProcAddress ( hKernel32, "LoadLibraryA" ) ),
                                        pLibPathRemote,
                                        0,
                                        NULL);

        if ( hThread == 0 )
        {
            return 0;
        }


    } __finally {
        VirtualFreeEx( hProcess, pLibPathRemote, strlen ( szLibPath ) + 1, MEM_RELEASE );
    }

    /*  We wait for the created remote thread to finish executing. When it's done, the DLL
        is loaded into the game's userspace, and we can destroy the thread-handle. We wait
        5 seconds which is way longer than this should take to prevent this application
        from deadlocking if something goes really wrong allowing us to kill the injected
        game executable and avoid user inconvenience.*/
    WaitForSingleObject ( hThread, INFINITE );

    /* Get the handle of the remotely loaded DLL module */
    DWORD hLibModule = 0;
    GetExitCodeThread ( hProcess, &hLibModule );

    // Wait for the LoadLibrary thread to finish
    WaitForSingleObject( hThread, INFINITE );

    /* Clean up the resources we used to inject the DLL */
    VirtualFreeEx (hProcess, pLibPathRemote, strlen ( szLibPath ) + 1, MEM_RELEASE );

    /* Success */
    return ( HINSTANCE )( 1 );
}

bool CheckFileExist(char filename[255])
{
	using namespace std;
	fstream file;
	file.open(filename,ios::in);
	if(file) 
		return true;
	return false;
	file.close();

}

bool IsGameRunning()
{
	HWND hWnd = false;
	HWND hWnd2 = false;
	hWnd = FindWindow(NULL, "Mafia");
	hWnd2 = FindWindow(NULL, "Mafia ..zzZZ");
	if(hWnd2 != NULL)
	{
		return true;
	}
	else if(hWnd != NULL)
	{
		return true;
	}
	else 
	{
	return false;
	}
}