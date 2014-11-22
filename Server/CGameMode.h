#ifndef CGAMEMODE_H
#define CGAMEMODE_H
class CGameMode
{
private:
	char pGamemodeName[250];
public:
	bool LoadGameMode(char*);
	bool UnloadGameMode(char*);

	void	SetName(char* name);
	char*	GetName();
};

#endif