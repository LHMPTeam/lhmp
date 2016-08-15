// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

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
public:
	CGameMode();
	// Load gamemode @name
	bool LoadGameMode(char* name);
	// Unloads currently running gamemode
	bool UnloadGameMode();
	// Reloads currently running gamemode
	void ReloadGameMode();

	// Returns the name of currently running gamemode
	char*	GetName();

	// TODO: eventually not used 
	// Returns  0 - OK
	//			1 - failed to compile
	//			2 - failed to safe file
	//			3 - other
	int CompileGameMode(char * in,char *out);

	// Send out the list of clientscripts used in gamemode to @client
	void SendClientScripts(RakNet::SystemAddress client);
private:
	char pGamemodeName[250];
	ClientScript* clientPool;
	int clientPoolSize;

	// Add @scriptname into the list of streamed files
	void	AddClientScript(char* scriptname);

	// Sets gamemode name attribute (visible in Server Query process)
	void	SetName(char* name);
};

#endif