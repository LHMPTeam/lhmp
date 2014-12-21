#include "CCore.h"
#include "CGameMode.h"
#include "../shared/tools.h"
#include <iostream>

#include "squirrelheads.h"

extern CCore *g_CCore;
bool CGameMode::LoadGameMode(char* name)
{
	this->clientPoolSize = 0;
	this->clientPool = 0;
	SetName("default");
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
						}
						else
						{
							g_CCore->GetLog()->AddNormalLog("[!] Failed to load script : %s", pointer + delim + 1);
						}
					}
					else if (strcmp(pointer, "CLIENT") == 0)
					{
						g_CCore->GetLog()->AddNormalLog("[!] Client script load %s", pointer + delim + 1);
						char newname[255];
						sprintf(newname, "%s.bak", pointer + delim + 1);
						int result = this->CompileGameMode(pointer + delim + 1, newname);
						//int result = 3;
						if (result == 0)
						{
							char path[500];
							sprintf(path, "gamemodes/%s/%s", name, newname);
							FILE* pFile = fopen(path, "rb");
							if (pFile)
							{
								g_CCore->GetLog()->AddNormalLog("[!] Client script %s compiled / added to files pool", pointer + delim + 1);
								g_CCore->GetFileTransfer()->AddFile(pointer + delim + 1, pFile);
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
	g_CCore->GetLog()->AddNormalLog("Gamemode's been successfully loaded.");
	return true;
}

bool CGameMode::UnloadGameMode(char* name)
{
	// unloads all Squirrel scripts
	g_CCore->GetScripts()->UnloadAll();
	g_CCore->GetFileTransfer()->Reset();
	g_CCore->SetDefaultMap("freeride");
	return true;
}

void CGameMode::ReloadGameMode()
{
	char tempname[500];
	strcpy(tempname, this->GetName());

	this->UnloadGameMode(tempname);
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