#include "CCrashHandler.h"
#include "CCore.h"
#include <psapi.h>
#include <Windows.h>
#include "../shared/version.h"
#include <fstream>
#include <iostream>

extern CCore* g_CCore;
void GetAdressAsModule(DWORD addr, char* outbuffer)
{
	HMODULE hMods[1024];
	HANDLE hProcess = GetCurrentProcess();
	DWORD cbNeeded;
	unsigned int i;
	DWORD close = 0x0;

	if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
		{

			if (addr - (DWORD)hMods[i] < addr - close)
				close = (DWORD)hMods[i];
		}

		if (close != NULL)
		{
			TCHAR szModName[MAX_PATH];
			if (GetModuleBaseName(hProcess, (HMODULE)close, szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				sprintf(outbuffer, "%s+0x%p", szModName, (addr - close));
				return;
			}
		}
		else {
			sprintf(outbuffer, "UNKNOWN");
		}
	}
}
/**
	Handles exception call.
	
	Saves register information on disk and run CrashHandler.exe,
	* which sends crash report to our server
*/
LONG	HandleIt(struct _EXCEPTION_POINTERS * ExceptionInfo)
{
	char buff[500];
	char address[100];
	GetAdressAsModule((DWORD)ExceptionInfo->ExceptionRecord->ExceptionAddress, address);
	sprintf(buff, "Crash occured at address: %p Code: %x \nRegisters: \n"
		"EAX: %p \tECX: %p \n"
		"EDX: %p \tEBX: %p \n"
		"ESP: %p \tEBP: %p \n"
		"ESI: %p \tEDI : %p \n"
		"Module: %s\n"
		"You will find these information in lhmp/crashdump.txt",ExceptionInfo->ExceptionRecord->ExceptionAddress, 
		ExceptionInfo->ExceptionRecord->ExceptionCode, ExceptionInfo->ContextRecord->Eax,
		ExceptionInfo->ContextRecord->Ecx, ExceptionInfo->ContextRecord->Edx, ExceptionInfo->ContextRecord->Ebx,
		ExceptionInfo->ContextRecord->Esp, ExceptionInfo->ContextRecord->Ebp, ExceptionInfo->ContextRecord->Esi,
		ExceptionInfo->ContextRecord->Edi,address);
	g_CCore->GetCrashHandler()->SaveDumpOnDisk(buff);

	STARTUPINFOA siStartupInfo;
	PROCESS_INFORMATION piProcessInfo;
	memset(&siStartupInfo, 0, sizeof(siStartupInfo));
	memset(&piProcessInfo, 0, sizeof(piProcessInfo));
	siStartupInfo.cb = sizeof(siStartupInfo);

	sprintf(buff, "CrashHandler.exe %p|%p@%p@%p@%p@%p@%p@%p@%p|%s|%s",
		ExceptionInfo->ExceptionRecord->ExceptionAddress,
		ExceptionInfo->ContextRecord->Eax,
		ExceptionInfo->ContextRecord->Ecx, ExceptionInfo->ContextRecord->Edx, ExceptionInfo->ContextRecord->Ebx,
		ExceptionInfo->ContextRecord->Esp, ExceptionInfo->ContextRecord->Ebp, ExceptionInfo->ContextRecord->Esi,
		ExceptionInfo->ContextRecord->Edi, address,LHMP_VERSION_TEST_HASH);
	if (!CreateProcessA(NULL,
		buff, 0, 0, false,
		CREATE_SUSPENDED, 0, 0,
		&siStartupInfo, &piProcessInfo)) {
		MessageBoxA(NULL, "Creating proccess failed !", "Error", MB_OK);
	}
	DWORD pId = piProcessInfo.dwProcessId;
	ResumeThread(piProcessInfo.hThread);
	return 0;
}
LONG WINAPI CrashExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	HandleIt(ExceptionInfo);
	return EXCEPTION_EXECUTE_HANDLER;
}

void	CCrashHandler::Prepare()
{
	SetUnhandledExceptionFilter(CrashExceptionFilter);
}

void	CCrashHandler::SaveDumpOnDisk(char* str)
{
	std::fstream zapis;
	zapis.open("lhmp/crashdump.txt", std::ios::app);
	zapis << "---" << std::endl;
	zapis << "MINI DUMP"<< std::endl;
	zapis << str << std::endl;
	zapis.close();
}

void CCrashHandler::SendReport(char* report){
	TCPInterface TCP;
	if (TCP.Start(0, 0) == false)
		return;
	SystemAddress server;
	server = TCP.Connect("lh-mp.eu", 80, true);
	if (server != UNASSIGNED_SYSTEM_ADDRESS)			// if we are connected
	{
		std::string mod = report;
		for (size_t pos = mod.find(' ');
			pos != std::string::npos;
			pos = mod.find(' ', pos))
		{
			mod.replace(pos, 1, "+");
		}

		std::string nick = g_CCore->GetLocalPlayer()->GetNickname();
		for (size_t pos = nick.find(' ');
			pos != std::string::npos;
			pos = nick.find(' ', pos))
		{
			nick.replace(pos, 1, "+");
		}

		char postRequest[2048] = "";
		
		RakNet::TimeMS TS = RakNet::GetTimeMS();

		char data[1024] = "";
		sprintf_s(data, "hash=%s&time=%d&dump=%s&nick=%s", LHMP_VERSION_TEST_HASH, (int)TS, mod.c_str(),nick.c_str());
		sprintf_s(postRequest,
			"POST /query/crash.php HTTP/1.1\r\n"
			"Content-Type: application/x-www-form-urlencoded\r\n"
			"Content-Length: %d\r\n"
			"Host: lh-mp.eu\r\n\r\n"
			"%s\r\n", strlen(data), data);
		TCP.Send(postRequest, strlen(postRequest), server, false);
		while (TCP.ReceiveHasPackets() == false)
			RakSleep(10);
		RakNet::Packet* pack = TCP.Receive();
	}
}