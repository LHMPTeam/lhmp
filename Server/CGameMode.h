/**
Lost Heaven Multiplayer

Purpose: loads, unloads and reloads gamemodes

@author Romop5
@version 1.0 1/9/14
*/

#ifndef CGAMEMODE_H
#define CGAMEMODE_H

struct ClientScript
{
	char name[255];
	ClientScript* next;
	ClientScript(char* _input)
	{
		sprintf(name, "%s", _input);
		this->next = NULL;
	}
};
class CGameMode
{
private:
	char pGamemodeName[250];
	ClientScript* clientPool;
	int clientPoolSize;
public:
	CGameMode();

	void	AddClientScript(char*);
	// Load gamemode @name
	bool LoadGameMode(char* name);
	bool UnloadGameMode();
	void ReloadGameMode();

	// Returns  0 - OK
	//			1 - failed to compile
	//			2 - failed to safe file
	//			3 - other
	int CompileGameMode(char * in,char *out);

	void	LoadGameFiles();
	void	SetName(char* name);
	char*	GetName();

	// Send out all client-script names to client
	void SendClientScripts(RakNet::SystemAddress client);

};

#endif