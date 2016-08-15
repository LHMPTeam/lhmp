// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>
#include <string.h>
#include <iostream>

void ACScanFromPath(WCHAR cPath[MAX_PATH]);
void ACScanModules();
void ACMainThread();
void ACSpeedhack();
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

HANDLE hProcessSelf;

// Blacklisted checksums
DWORD dwACBlacklist[] = {
	279040, // Mafia.dll (Mafiacon)
	763392, //MHook.dll (Mafiacon)
	153600, //MafiaHack.exe
	315392, //fdx - maft.exe
	1464320, //Mafia + 10trn.exe
	331776, //mafia - trn.exe
	196608 //trainer.exe (1.0 trainer)
};