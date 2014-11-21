#include "CCore.h"
#include "CGameMode.h"
#include "../shared/tools.h"
#include <iostream>

extern CCore *g_CCore;
bool CGameMode::LoadGameMode(char* name)
{
	char resource[255];
	sprintf(resource, "gamemodes/%s/resources.txt",name);
	if (Tools::fileExists(resource))
	{
		// load scripts
		FILE* file = fopen(resource, "r");
		if (file != NULL)
		{
			char buff[256];
			while (feof(file) == 0)
			{
				fgets(buff, 255, file);
				char* script = strtok(buff, "\n");
				char path[500];
				sprintf(path, "gamemodes/%s/%s", name, script);
				if (Tools::fileExists(path))
				{
					g_CCore->GetScripts()->LoadScript(path);
				}
				else
				{
					g_CCore->GetLog()->AddNormalLog("[!] Failed to load script : %s",buff);
				}
			}
			fclose(file);
		}
		else
		{
			g_CCore->GetLog()->AddNormalLog("[!] Gamemode has failed to load.");
			return false;
		}
	}
	else
	{

		g_CCore->GetLog()->AddNormalLog("[!] Gamemode has failed to load.");
		return false;
	}
	g_CCore->GetLog()->AddNormalLog("Gamemode's been successfully loaded.");
	return true;
}

bool CGameMode::UnloadGameMode(char* name)
{
	char resource[255];
	sprintf(resource, "gamemodes/%s/resources.txt", name);
	if (Tools::fileExists(resource))
	{
		// load scripts
		char buff[256];
		FILE* file = fopen(resource, "r");
		if (file != NULL)
		{
			while (feof(file) == 0)
			{
				fgets(buff, 255, file);
				char* script = strtok(buff, "\n");
				char path[500];
				sprintf(path, "gamemodes/%s/%s", name, script);
				if (Tools::fileExists(path))
				{
					g_CCore->GetScripts()->UnloadScript(path);
				}
				else
				{
					g_CCore->GetLog()->AddNormalLog("[!] Failed to unload script: %s", buff);
				}
			}
			fclose(file);
		}
		else
		{
			g_CCore->GetLog()->AddNormalLog("[!] Gamemode has failed to unload.");
			return false;
		}
	}
	else
	{

		g_CCore->GetLog()->AddNormalLog("[!] Gamemode has failed to unload.");
		return false;
	}
	g_CCore->GetLog()->AddNormalLog("Gamemode's been successfully unloaded.");
	return true;
}