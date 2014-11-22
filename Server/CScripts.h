/*#include "CScript.h"
#include "squirrelheads.h"
class CScripts
{
private:
	char		szName[255];
	CScript*	slot[100];
public:
	void		LoadScript(char*);
	void		ReloadScripts();
	void		Call(const char *, int, SQObject*);
};*/


#pragma once
/*#include "../vendor/squirrel/squirrel.h"
#include "../vendor/squirrel/sqvm.h"
#include "CConfig.h"
#include "CScript.h"
*/
#include "CScript.h"
#include "squirrelheads.h"
#define MAX_SCRIPTS 10

class CScripts
{
private:
	CScript * m_pScripts[MAX_SCRIPTS];
	char m_szScriptNames[MAX_SCRIPTS][256];

public:
	CScripts();
	~CScripts();

	bool LoadScript(const char * szScriptName);
	bool UnloadScript(const char * szScriptName);
	//bool LoadFromConfig(CConfig * pConfig);

	CScript * GetScript(int scriptID) {
		if (!m_pScripts[scriptID]) return NULL;
		return m_pScripts[scriptID];
	};

	void Call(const char * szFunc, int iArgCount, SQObject * pArguments);

	void onServerInit();
	void onServerExit();
	bool onPlayerText(int, const char*);
	void onPlayerCommand(int, const char *,const char *);
	void onPlayerConnect(int);
	void onPlayerDisconnect(int);
	void onPlayerEnterVehicle(int,int,int);
	void onPlayerExitVehicle(int, int);
	void onPlayerSpawn(int);
	void onPlayerIsKilled(int,int);
	void onServerTick(int);
	void onServerTickSecond(int);
	void onPlayerKeyPressed(int,int);

	void onPlayerShoot(int, int);
	void onPlayerHit(int,int);
	void onPickupTaken(int, int);
	//void onPlayerTh(int);

};