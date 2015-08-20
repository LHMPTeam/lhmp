#include "CCore.h"
#include "CGameMode.h"
#include "../shared/tools.h"
#include <iostream>

#include "squirrelheads.h"

extern CCore *g_CCore;

CGameMode::CGameMode()
{
	sprintf(this->pGamemodeName, "default");
}

bool CGameMode::LoadGameMode(char* name)
{
	unsigned int successfulFiles = 0;
	this->clientPoolSize = 0;
	this->clientPool = 0;
	SetName(name);
	char resource[255];
	sprintf(resource, "gamemodes/%s/resources.txt",name);
	if (Tools::fileExists(resource))
	{
		// load scripts
		FILE* file = fopen(resource, "rb");
		if (file != NULL)
		{
			// File size
			fseek(file, 0, SEEK_END);   // non-portable
			int size = ftell(file);
			fseek(file, 0, SEEK_SET);

			char *fileContent = new char[size+1];
			fread(fileContent, sizeof(char), size, file);
			fileContent[size] = 0x0;

			char* pointer;
			pointer = strtok(fileContent, "\r\n\0");
			int lineNumber = 0;
			while(pointer != NULL)
			{
				lineNumber++;
				int len = strlen(pointer);
				int delim = -1;
				for (int i = 0; i < len; i++)
				{
					if (pointer[i] == ' ')
					{
						delim = i;
						break;
					}

				}
				if (delim == -1)
				{
					g_CCore->GetLog()->AddNormalLog("[!] Failed to parse resources.txt at line: %d  %s",lineNumber, pointer + delim + 1);
				}
				else {
					pointer[delim] = 0x0;
					if (strcmp(pointer, "SERVER") == 0)
					{
						char path[500];
						sprintf(path, "gamemodes/%s/%s", name, pointer+delim+1);
						if (Tools::fileExists(path))
						{
							g_CCore->GetScripts()->LoadScript(path);
							successfulFiles++;
						}
						else
						{
							g_CCore->GetLog()->AddNormalLog("[!] Failed to load script : %s", pointer + delim + 1);
						}
					}
					else if (strcmp(pointer, "CLIENT") == 0)
					{
						//g_CCore->GetLog()->AddNormalLog("[!] Loading client script '%s'", pointer + delim + 1);

						char path[500];
						char newname[255];
						// path to original .nut file
						sprintf(path, "gamemodes/%s/%s", name, pointer + delim + 1);
						// generate compiled .nut file name and store it into newname
						sprintf(newname, "gamemodes/%s/%s", name, pointer + delim + 1);
						//sprintf(newname, "gamemodes/%s/%s.bak", name, pointer + delim + 1);	// .bak file, not used
						// try to compile .nut file
						
						//int result = this->CompileGameMode(path, newname);
						int result = 0; // SUCCESS
						// now store new file path
						//sprintf(path, "gamemodes/%s/%s", name, newname);

						// was compilation successful
						if (result == 0)
						{
							//g_CCore->GetLog()->AddNormalLog("[!] FOPEN %s", newname);

							FILE* pFile = fopen(newname, "rb");
							// can we open the recently compiled file (required for file transfer) ?
							if (pFile)
							{
								//g_CCore->GetLog()->AddNormalLog("[!] Client script %s compiled / added to files pool", pointer + delim + 1);

								g_CCore->GetFileTransfer()->AddFile(pointer + delim + 1, pFile);
								this->AddClientScript(pointer + delim + 1);
								successfulFiles++;
							}
							else {
								g_CCore->GetLog()->AddNormalLog("[!] Client script '%s' load failed to open", pointer + delim + 1);
							}
						}
						else {
							g_CCore->GetLog()->AddNormalLog("[!] Client script %s compile failed, ID: %d", pointer + delim + 1,result);
						}
					}
					else {
						char path[500];
						sprintf(path, "gamemodes/%s/%s", name, pointer + delim + 1);
						FILE* pFile = fopen(path, "rb");
						if (pFile)
						{
							g_CCore->GetLog()->AddNormalLog("[!] File '%s has been added to file transfer", pointer + delim + 1);
							g_CCore->GetFileTransfer()->AddFile(pointer + delim + 1, pFile);
							successfulFiles++;
						}
						else {
							g_CCore->GetLog()->AddNormalLog("[!] File '%s' has been skipped...", pointer + delim + 1);
						}
					}
					pointer[delim] = ' ';
				}
				pointer = strtok(NULL, "\r\n\0");
			}
			delete[] fileContent;
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
	SetName(name);
	g_CCore->GetLog()->AddNormalLog("Gamemode '%s' been successfully loaded. \n%d file(s) processed",name,successfulFiles++);
	return true;
}

bool CGameMode::UnloadGameMode()
{
	// unloads all Squirrel scripts
	g_CCore->GetScripts()->UnloadAll();
	g_CCore->GetFileTransfer()->Reset();
	g_CCore->SetDefaultMap("freeride");

	// Delete all players
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		g_CCore->GetPlayerPool()->Delete(i);
	}

	// Delete all vehicles
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		g_CCore->GetVehiclePool()->Delete(i);
	}

	// reset the doors' pool
	g_CCore->GetDoorPool()->Reset();

	// reset all files
	g_CCore->GetFileTransfer()->Reset();

	// reset pickup pool
	g_CCore->GetPickupPool()->Reset();
	return true;
}

void CGameMode::ReloadGameMode()
{
	char tempname[500];
	strcpy(tempname, this->GetName());

	this->UnloadGameMode();
	this->LoadGameMode(tempname);

	printf("Fucking name '%s' \n", tempname);

}

void	CGameMode::SetName(char* name)
{
	sprintf(this->pGamemodeName, "%s", name);
}
char*	CGameMode::GetName()
{
	return this->pGamemodeName;
}

void compile_error_handler(HSQUIRRELVM v, const SQChar* desc, const SQChar* source, SQInteger line, SQInteger column)
{
	//Log << "Error in " << source << ":" << line << ":" << column << ", " << desc << std::endl;
	g_CCore->GetLog()->AddNormalLog("Error in %s:%d:%d , %s", source, line, column, desc);
}

int		CGameMode::CompileGameMode(char* in, char* out)
{
	HSQUIRRELVM v = sq_open(1024);
	if (!v) {
		//Log << "Could not open Squirrel VM" << std::endl;
		return 3;
	}

	// set compile error handler
	sq_setcompilererrorhandler(v, compile_error_handler);

	// compile source file
	if (SQ_FAILED(sqstd_loadfile(v, (const SQChar*)in, SQTrue))) {
		//Log << "Could not compile source file " << argv[1] << std::endl;
		sq_close(v);
		return 1;
	}

	// serialize closure containing the source
	if (SQ_FAILED(sqstd_writeclosuretofile(v, (const SQChar*)out))) {
		//Log << "Could not serialize closure" << std::endl;
		sq_close(v);
		return 2;
	}
	sq_close(v);
	return 0;
}


void	CGameMode::AddClientScript(char* script)
{

	ClientScript* pointer = this->clientPool;
	if (pointer == NULL)
	{
		this->clientPool = new ClientScript(script);
	} else {
		while (pointer->next != NULL)
			pointer = pointer->next;
		pointer->next = new ClientScript(script);
	}
	this->clientPoolSize++;
}


// Send the whole list of client-side scripts to @client
void CGameMode::SendClientScripts(RakNet::SystemAddress client)
{
	RakNet::BitStream bsOut; 
	bsOut.Write((RakNet::MessageID)ID_SCRIPSTLIST);
	bsOut.Write((unsigned short)this->clientPoolSize);
	ClientScript* pointer = this->clientPool;

	while (pointer != NULL)
	{
		g_CCore->GetLog()->AddNormalLog("Sending script %s\n",pointer->name);
		bsOut.Write(pointer->name);
		pointer = pointer->next;
	}
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, client, false);
}