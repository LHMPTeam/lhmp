// inject.cpp

#include "inject.h"

HMODULE InjectDLL(DWORD ProcessID, char* dllName)
{
   HANDLE Proc;
   HANDLE Thread;
   char buf[50]={0};
   LPVOID RemoteString, LoadLibAddy;
   HMODULE hModule = NULL;
   DWORD dwOut;

   if(!ProcessID)
	  return false;

   Proc = OpenProcess(PROCESS_ALL_ACCESS, 0, ProcessID);

   if(!Proc)
   {
	  sprintf_s(buf, "OpenProcess() failed: %d", GetLastError());
	  MessageBoxA(NULL, buf, "Loader", NULL);
	  return false;
   }

   LoadLibAddy = (LPVOID)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
   if (!LoadLibAddy) {
		return false;
   }


   RemoteString = (LPVOID)VirtualAllocEx(Proc, NULL, strlen(dllName), MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
   if (!RemoteString) {
	   return false;
   }

   if (!WriteProcessMemory(Proc, (LPVOID)RemoteString, dllName, strlen(dllName), NULL)) {
	   return false;
	}

   Thread = CreateRemoteThread(Proc, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibAddy, (LPVOID)RemoteString, NULL, NULL);   
   if (!Thread) {
		return false;
   } else {
		while(GetExitCodeThread(Thread, &dwOut)) {
			if(dwOut != STILL_ACTIVE) {
				hModule = (HMODULE)dwOut;
				break;
			}
		}
	}

   CloseHandle(Thread);
   CloseHandle(Proc);

   return hModule;
}