#include <Windows.h>


int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	char gameDirectoryPath[MAX_PATH];
	GetModuleFileNameA(0, gameDirectoryPath, MAX_PATH);
	strrchr(gameDirectoryPath, '\\')[0] = 0;

	char gameExecutablePath[MAX_PATH];
	strcpy_s(gameExecutablePath, gameDirectoryPath);
	strcat_s(gameExecutablePath, "\\Game.exe");

	char injectLibraryPath[MAX_PATH];
	strcpy_s(injectLibraryPath, gameDirectoryPath);
	strcat_s(injectLibraryPath, "\\lhmp.dll");
	PROCESS_INFORMATION processInformation = { 0 };
	STARTUPINFOA startupInfo = { 0 };
	if (CreateProcessA(gameExecutablePath,
		0, 0, 0,
		false,
		CREATE_SUSPENDED,
		NULL,
		gameDirectoryPath,
		(LPSTARTUPINFOA)&startupInfo,
		&processInformation) == FALSE)
	{
		DWORD errorId = GetLastError();
		if (errorId == ERROR_ELEVATION_REQUIRED)
		{
			MessageBoxA(NULL, "Please re-run app as administrator!", "Launch failed", MB_OK);
		}
		return 1;
	}

	LPTHREAD_START_ROUTINE kernelStartRoutine = (LPTHREAD_START_ROUTINE)(GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"));
	void* libraryRemotePath = VirtualAllocEx(processInformation.hProcess, NULL, strlen(injectLibraryPath), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	WriteProcessMemory(processInformation.hProcess, libraryRemotePath, injectLibraryPath, strlen(injectLibraryPath), NULL);
	HANDLE libraryHandle = CreateRemoteThread(processInformation.hProcess,
		NULL,
		NULL,
		kernelStartRoutine,
		libraryRemotePath,
		NULL,
		NULL);
	WaitForSingleObject(libraryHandle, INFINITE);
	VirtualFreeEx(processInformation.hProcess, libraryRemotePath, strlen(injectLibraryPath), MEM_RELEASE);
	CloseHandle(libraryHandle);
	CloseHandle(processInformation.hProcess);
	ResumeThread(processInformation.hThread);
	return 0;
}
