#include <iostream>
#include <stdio.h>
#include "CCore.h"
#include <string>
#include "squirrelheads.h"
#include "CConfig.h"

#include "../shared/tools.h"

#ifdef _MSC_VER
#pragma comment (lib ,"squirrel.lib")
#pragma comment (lib ,"sqstdlib.lib")
#endif

CCore	*g_CCore = NULL;
int main()
{
	CConfig* cfg = new CConfig();
	char*	server_name	= cfg->GetCString("servername", "Default Lost Heaven Server");
	int		server_port = cfg->GetInt("server_port", 27015);
	int		max_players = cfg->GetInt("maxplayers", 16);
	char*	gamemode	= cfg->GetCString("gamemode", "default");
	char*	mode		= cfg->GetCString("mode", "Default mode");
	bool	visible		= cfg->GetBool("visible", 1);
	delete cfg;
	std::string startpos = "-1985.966675 -5.037054 4.284860";
	max_players = Tools::Clamp(max_players, 0, MAX_PLAYERS);

#ifdef _WIN32
	SetConsoleTitle(server_name);
#endif
	//g_CCore->GetLog()->AddNormalLog("\t\t\t[LOST-HEAVEN-MP]==[Win32/Alpha]");
	
	g_CCore->GetLog()->AddNormalLog("===============================================================================");
	g_CCore->GetLog()->AddNormalLog(" _        _   _          _      _    _____			");
	g_CCore->GetLog()->AddNormalLog("| |      | | | |    _   | |\\  /| |  |     \\				");
	g_CCore->GetLog()->AddNormalLog("| |      | |_| |   |_|  | | \\/ | |  |  ___/		Version	");
	g_CCore->GetLog()->AddNormalLog("| |      | |_| |    _   | |\\__/| |  | |			-0.1-	");
	g_CCore->GetLog()->AddNormalLog("| |____  | | | |   |_|  | |    | |  | |			-Windows-");
	g_CCore->GetLog()->AddNormalLog("|______| |_| |_|        |_|    |_|  |_|					");
	g_CCore->GetLog()->AddNormalLog("");
	g_CCore->GetLog()->AddNormalLog("===============================================================================");
	g_CCore->GetLog()->AddNormalLog("Server Name: %s", server_name);
	g_CCore->GetLog()->AddNormalLog("Server Port: %d", server_port);
	g_CCore->GetLog()->AddNormalLog("Max.Players: %d", max_players);
	g_CCore->GetLog()->AddNormalLog("===============================================================================");

	CCore CCore;
	g_CCore = &CCore;
	if (CCore.Init(server_port, max_players, startpos, server_name, mode, visible) == true)
	{
		g_CCore->GetLog()->AddNormalLog("===============================================================================");
		g_CCore->GetLog()->AddNormalLog("Loading gamemode '%s'", gamemode);

		CCore.GetGameMode()->LoadGameMode(gamemode);
		CCore.GetScripts()->onServerInit();

		g_CCore->GetLog()->AddNormalLog("===============================================================================");
		g_CCore->GetLog()->AddNormalLog("Server has started...");
		while(CCore.IsRunning())
		{
			CCore.Pulse();
		}

		CCore.GetScripts()->onServerExit();
	}
	else
	{
		g_CCore->GetLog()->AddNormalLog("Server initialization failed, server will shutdown");
		std::cin.get();
		std::cin.get();
	}
}
