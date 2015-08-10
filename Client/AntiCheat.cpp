#include "AntiCheat.h"

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	char title[80];
	std::string str(title);
	std::string stk("Cheat Engine");

	GetWindowText(hwnd, title, sizeof(title));
	str = title;

	std::size_t found = str.find(stk);
	if (found != std::string::npos) {
		return false;
	}

	return true;
}

void ACScanFromPath(WCHAR cPath[MAX_PATH]) {
	DWORD dwRead;
	DWORD dwChecksum;

	// Create file handle
	HANDLE hFile = CreateFileW(cPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hFile != INVALID_HANDLE_VALUE) {
		DWORD dwSize = GetFileSize(hFile, NULL);

		if (dwSize != INVALID_FILE_SIZE) {
			// Allocate some memory for us
			LPVOID lpBuffer = GlobalAlloc(GMEM_FIXED, dwSize);

			if (ReadFile(hFile, (BYTE *)lpBuffer, dwSize, &dwRead, NULL)) {
				// Read image PE-headers
				PIMAGE_DOS_HEADER piDH = (PIMAGE_DOS_HEADER)lpBuffer;
				PIMAGE_NT_HEADERS piNH = (PIMAGE_NT_HEADERS)((BYTE*)piDH + piDH->e_lfanew);
				PIMAGE_OPTIONAL_HEADER piOH = (PIMAGE_OPTIONAL_HEADER)&piNH->OptionalHeader;

				// Calculate checksum
				dwChecksum = piOH->SizeOfCode + piOH->SizeOfImage;

				if (dwChecksum > 0) {
					// Search for checksum in the blacklist
					int i;

					for (i = 0; i < (sizeof(dwACBlacklist) / sizeof(*dwACBlacklist)); i++) {
						if (dwChecksum == dwACBlacklist[i]) {
							// Cheat was found, for now terminate the game
							TerminateProcess(GetCurrentProcess(), 0);
						}
					}
				}
			}

			// Then free it
			GlobalFree(lpBuffer);
		}
	}

	CloseHandle(hFile);
}

void ACScanModules() {
	HMODULE hModules[1024];
	DWORD dwCBNeeded;

	if (hProcessSelf == NULL) {
		hProcessSelf = GetCurrentProcess();
	};

	unsigned int i;

	if (EnumProcessModules(hProcessSelf, hModules, sizeof(hModules), &dwCBNeeded)) {
		for (i = 0; i < (dwCBNeeded / sizeof(HMODULE)); i++) {
			WCHAR cPath[MAX_PATH];

			if (GetModuleFileNameExW(hProcessSelf, hModules[i], cPath, MAX_PATH)) { // Ex?
				// Scan current module
				ACScanFromPath(cPath);
			}

			Sleep(100);
		}
	}
}

void ACScanProcesses() {
	DWORD dwProcesses[1024];
	DWORD dwCBNeeded;
	DWORD dwCProcesses;
	unsigned int i;

	if (EnumProcesses(dwProcesses, sizeof(dwProcesses), &dwCBNeeded)) {
		dwCProcesses = dwCBNeeded / sizeof(DWORD);

		for (i = 0; i < dwCProcesses; i++) {
			if (dwProcesses[i] != 0) {
				WCHAR cPath[MAX_PATH];
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcesses[i]);

				if (hProcess != NULL) {
					HMODULE hMod;
					DWORD dwCBNeededMod;

					if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &dwCBNeededMod)) {
						GetModuleFileNameExW(hProcess, hMod, cPath, MAX_PATH);

						// Scan current process
						ACScanFromPath(cPath);
					}
				}

				CloseHandle(hProcess);
			}

			Sleep(100);
		}
	}
}

void ACSpeedhack() {
	int time = GetTickCount();

	Sleep(100);

	int timeElapsed = GetTickCount() - time;

	if (timeElapsed >= 115) {
		// Speedhacking, kill process
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

void ACCheatEngine() {
	BOOL result = EnumWindows(EnumWindowsProc, NULL);

	if (result == FALSE) {
		// CE open
		TerminateProcess(GetCurrentProcess(), 0);
	}
}

void ACMainThread() {
	while (1) {
		ACScanProcesses();
		ACScanModules();
		ACSpeedhack();
		ACCheatEngine();

		Sleep(1000);
	}
}