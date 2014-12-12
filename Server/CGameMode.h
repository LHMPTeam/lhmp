#ifndef CGAMEMODE_H
#define CGAMEMODE_H

struct ClientScript
{
	char name[255];
	ClientScript* next;
	ClientScript(char* _input)
	{
		sprintf(name, "%s", _input);
	}
};
class CGameMode
{
private:
	char pGamemodeName[250];
	ClientScript* clientPool;
	int clientPoolSize = 0;
public:
	void	AddClientScript(char*);
	bool LoadGameMode(char*);
	bool UnloadGameMode(char*);
	// Returns  0 - OK
	//			1 - failed to compile
	//			2 - failed to safe file
	//			3 - other
	int CompileGameMode(char * in,char *out);

	void	LoadGameFiles();
	void	SetName(char* name);
	char*	GetName();

};

#endif