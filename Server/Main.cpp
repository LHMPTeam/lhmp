#include <iostream>
#include <stdio.h>
#include "CCore.h"
#include <string>
#include "squirrelheads.h"
#include "CConfig.h"

#include "../shared/tools.h"
#include "../shared/version.h"

#ifdef _MSC_VER
#pragma comment (lib ,"squirrel.lib")
#pragma comment (lib ,"sqstdlib.lib")
#endif

CCore	*g_CCore = NULL;
// Setups LHMP server
int main()
{
	// Load config.txt
	CConfig* cfg = new CConfig();
	char*	server_name	= cfg->GetCString("servername", "Default Lost Heaven Server");
	int		server_port = cfg->GetInt("server_port", 27015);
	int		tick_delay = cfg->GetInt("tick_delay", 0);
	int		max_players = cfg->GetInt("maxplayers", 16);
	char*	gamemode	= cfg->GetCString("gamemode", "default");
	char*	mode		= cfg->GetCString("mode", "Default mode");
	char*	password	= cfg->GetCString("password", "");
	bool	visible		= cfg->GetBool("visible", 1);
	char*	websiteurl = cfg->GetCString("website", "(No URL!)");
	delete cfg;
	std::string startpos = "-1985.966675 -5.037054 4.284860";
	max_players = Tools::Clamp(max_players, 1, MAX_PLAYERS);

	// dump server info & variables
	g_CCore->GetLog()->AddNormalLog("===============================================================================");
	g_CCore->GetLog()->AddNormalLog(" _        _   _          _      _    _____			");
	g_CCore->GetLog()->AddNormalLog("| |      | | | |    _   | |\\  /| |  |     \\				");
	g_CCore->GetLog()->AddNormalLog("| |      | |_| |   |_|  | | \\/ | |  |  ___/		Version	");
	g_CCore->GetLog()->AddNormalLog("| |      | |_| |    _   | |\\__/| |  | |			-1.0-	");
	g_CCore->GetLog()->AddNormalLog("| |____  | | | |   |_|  | |    | |  | |			-%s-", LHMP_PLATFORM);
	g_CCore->GetLog()->AddNormalLog("|______| |_| |_|        |_|    |_|  |_|					");
	g_CCore->GetLog()->AddNormalLog("");
	g_CCore->GetLog()->AddNormalLog("===============================================================================");
	g_CCore->GetLog()->AddNormalLog("Server Name: %s", server_name);
	g_CCore->GetLog()->AddNormalLog("Server Port: %d\t\t\tMax.Players: %d", server_port, max_players);
	g_CCore->GetLog()->AddNormalLog("Gamemode:    %s\t\t\tURL:         %s", gamemode, websiteurl);
	g_CCore->GetLog()->AddNormalLog("Password:    %s\t\t\t\tTick delay:  %i", (strlen(password) > 0) ? "yes" : "no", tick_delay);
	g_CCore->GetLog()->AddNormalLog("===============================================================================");

	// run CCore instance
	CCore CCore;
	g_CCore = &CCore;
	// if server inits correctly
	int startupResult;
	if ((startupResult = CCore.Init(server_port, max_players, startpos, server_name, mode, visible, websiteurl, password)) == STARTUP_SUCCESS)
	{
		g_CCore->GetLog()->AddNormalLog("Core has been initilaized successfully.");
		g_CCore->GetLog()->AddNormalLog("===============================================================================");

		g_CCore->tickDelay = tick_delay;

		// now try to load gamemode
		if (CCore.GetGameMode()->LoadGameMode(gamemode) == false)
		{
			g_CCore->GetLog()->AddNormalLog("Loading of '%s' has failed - no gamemode loaded !",gamemode);
		}

		// load banlist
		g_CCore->GetLog()->AddNormalLog("===============================================================================");
		g_CCore->GetBanSystem()->LoadBanlist();
		g_CCore->GetLog()->AddNormalLog("===============================================================================");

		// call Squirrel onServerInit callback
		CCore.GetScripts()->onServerInit();

		g_CCore->GetLog()->AddNormalLog("===============================================================================");
		g_CCore->GetLog()->AddNormalLog("Server has started...");

		g_CCore->GetMasterServer()->AddServerToMaster();

		// pulse CCore until server runs
		while(CCore.IsRunning())
		{
			CCore.Pulse();
		}

		CCore.GetScripts()->onServerExit();
	}
	else
	{
		// Server haven't set up correctly, wait for user imput and then terminate
		g_CCore->GetLog()->AddNormalLog("Server initialization failed, server will shutdown");
		switch (startupResult)
		{
		case STARTUP_NETWORK_FAILED:
			g_CCore->GetLog()->AddNormalLog("Reason: network init has failed");
			break;
		case STARTUP_QUERY_FAILED:
			g_CCore->GetLog()->AddNormalLog("Reason: query init has failed");
			break;
		}
		std::cin.get();
		std::cin.get();
	}
}
