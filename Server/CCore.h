/**
Lost Heaven Multiplayer

Purpose: core of server, main class

@author Romop5
@version 1.0 1/9/14
*/

#ifndef CCORE_H
#define	CCORE_H

#include "../shared/linux.h"

#include "CNetworkManager.h"
#include "CTickManager.h"
#include "CPlayerPool.h"
#include "CVehiclePool.h"
#include "CScripts.h"
#include "CConsole.h"
#include "CGameMode.h"
#include "CTimerPool.h"
#include "CDoorPool.h"
#include "CLog.h"
#include "CPickupPool.h"
#include "CFileTransfer.h"
#include "CBanSystem.h"
#include "CQueryServer.h"
#include "CMasterServer.h"

enum STARTUP_ERRORS
{
	STARTUP_SUCCESS,
	STARTUP_NETWORK_FAILED,
	STARTUP_QUERY_FAILED
};

class CCore
{
private:
	bool				m_bIsRunning;
	CBanSystem			m_cBanSystem;
	CNetworkManager		m_cNetworkManager;
	CTickManager		m_cTickManager;
	CPlayerPool			m_cPlayerPool;
	CVehiclePool		m_cVehiclePool;
	CScripts			m_cScripts;
	CConsole			m_cConsole;
	CGameMode			m_cGameMode;
	CTimerPool			m_cTimerPool;
	CDoorPool			m_cDoorPool;
	CPickupPool			m_cPickupPool;
	CLog				m_cLog;
	CFileTransfer		m_cFileTransfer;
	CQueryServer		m_cQueryServer;
	CMasterList			m_cMaster;
	char				m_cMapName[200];

public:
	CCore();
	~CCore();
	// Init server with params
	// Returns false if something went wrogn
	int		Init(int,int,std::string,std::string,std::string, int, char* website,char* password);
	// Tick callback 
	void	Pulse();
	// Returns whether server is running
	bool	IsRunning();
	// Tick callback called every 1000ms (1s)
	void	OnSecondElapsed();

	// Reloads actuall mode
	void	ReloadGamemode();
	void	ChangeModeTo(char* newmode);

	// return subclasses

	CNetworkManager*	GetNetworkManager();
	CTickManager*		GetTickManager();
	CPlayerPool*		GetPlayerPool();
	CVehiclePool*		GetVehiclePool();
	CScripts*			GetScripts();
	CConsole*			GetConsole();
	CGameMode*			GetGameMode();
	CTimerPool*			GetTimerPool();
	CDoorPool*			GetDoorPool();
	CPickupPool*		GetPickupPool();
	CLog*				GetLog();
	CFileTransfer*		GetFileTransfer();
	CBanSystem*			GetBanSystem();
	CQueryServer*		GetQueryServer();
	CMasterList*		GetMasterServer();

	void				SetDefaultMap(char*);
	char*				GetDefaultMap();
};
#endif