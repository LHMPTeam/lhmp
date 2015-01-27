/**
	Lost Heaven Multiplayer

	Purpose: Anti-cheat

	@author qbt
	@version 1.0 14/01/15
*/

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <psapi.h>

void ACScanFromPath(WCHAR cPath[MAX_PATH]);
void ACScanModules();
void ACMainThread();

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