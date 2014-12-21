#include "CConsole.h"
#include "CCore.h"

#include "../shared/tools.h"
#ifndef _WIN32
#include <pthread.h>
pthread_t tid;
#endif // _WIN32
extern CCore *g_CCore;

#ifdef _WIN32
void ConsoleThread(){
#else
void* ConsoleThread(void *arg){
#endif // _WIN32
	while (1)
	{
		g_CCore->GetConsole()->Tick();
		RakSleep(200);
	}
}
void CConsole::Init()
{
    #ifdef _WIN32
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ConsoleThread, 0, NULL, NULL);
	#else
	pthread_create(&tid,NULL,&ConsoleThread,NULL);
	#endif
}

void CConsole::Tick()
{
	char buff[256];
	std::cin.getline(buff, 255);
	std::cin.clear();

	char command[256];
	char varlist[512] = "";
	char *pch;
	pch = strchr(buff, ' ');
	if (pch == NULL)
	{
		memcpy(command, buff, strlen(buff));
	}
	else {
		memcpy(command, buff, (pch - buff));
		command[(pch - buff)] = '\0';
		memcpy(varlist, pch + 1, strlen(pch));
	}

	//printf("Entered command: '%s'\n", command);
	if (strcmp(command, "exit") == 0 || strcmp(command, "quit") == 0)
	{
		g_CCore->GetLog()->AddNormalLog("Exit");
		g_CCore->GetNetworkManager()->GetPeer()->Shutdown(100, 0, IMMEDIATE_PRIORITY);
		#ifdef _WIN32
		TerminateProcess(GetCurrentProcess(), 0);
		#else
		exit(0);
		#endif
	} else if (strcmp(command, "help") == 0)
	{
		g_CCore->GetLog()->AddNormalLog("List of commands:");
		g_CCore->GetLog()->AddNormalLog("-> help:\t this help");
		g_CCore->GetLog()->AddNormalLog("-> kick:\t kick player");
		g_CCore->GetLog()->AddNormalLog("-> exit:\t shutdown server");
		g_CCore->GetLog()->AddNormalLog("-> quit:\t shutdown server");
	} else if (strcmp(command, "kick") == 0)
	{
		if (strlen(varlist) == 0 || Tools::isStringNumeric(varlist) == false)
		{
			g_CCore->GetLog()->AddNormalLog("Usage: kick <ID>");
		}
		else
		{
			SystemAddress sa = g_CCore->GetNetworkManager()->GetSystemAddressFromID(atoi(varlist));
			g_CCore->GetNetworkManager()->GetPeer()->CloseConnection(sa, true);
		}
	}
	else if (strcmp(command, "msg") == 0)
	{
		if (strlen(varlist) == 0)
		{
			g_CCore->GetLog()->AddNormalLog("Usage: msg <some message to all players>");
		}
		else
		{
			//SystemAddress sa = g_CCore->GetNetworkManager()->GetSystemAddressFromID(atoi(varlist));
			//g_CCore->GetNetworkManager()->GetPeer()->CloseConnection(sa, true);
			char message[250];
			sprintf(message, "#ff0000[Admin]#fffFFF%s", varlist);
			g_CCore->GetNetworkManager()->SendMessageToAll(message);
		}
	}

	else if (strcmp(command, "reload") == 0)
	{
		g_CCore->ReloadGamemode();
	}

	else if (strcmp(command, "load") == 0)
	{
		if (strlen(varlist) == 0)
		{
			g_CCore->GetLog()->AddNormalLog("Usage: load <gamemode>");
		}
		else
		{
			g_CCore->ChangeModeTo(varlist);
		}
	}

	if (strcmp(command, "pl") == 0)
	{
		int pocet = 0;
		for (int i = 0; i < 100; i++)
		{
			CPlayer* player = g_CCore->GetPlayerPool()->Return(i);
			if (player != NULL)
				pocet++;
		}
		std::cout << "Count: " << pocet << std::endl;
	}
}
