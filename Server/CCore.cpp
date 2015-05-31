#include "CCore.h"

CCore::CCore()
{
	m_bIsRunning = true;
	sprintf(m_cMapName, "freeride");
}

CCore::~CCore()
{
}

bool CCore::IsRunning()
{
	return m_bIsRunning;
}

void CCore::Pulse()
{
	//m_cQueryServer.Tick();
	m_cNetworkManager.Pulse();
	m_cTickManager.Pulse();
	m_cTimerPool.Tick();
	m_cMaster.Pulse();
}

// pulse every 1000ms
void CCore::OnSecondElapsed()
{
	m_cNetworkManager.SendPingUpdate();
	m_cPickupPool.Tick();
}

int CCore::Init(int port,int players, std::string startpos, std::string svrname,std::string mode, int visible,char* website)
{
	if (m_cNetworkManager.Init(port, players, startpos, mode) == false)
		return STARTUP_NETWORK_FAILED;
	if (m_cQueryServer.StartServer(port + 1) == false)
		return STARTUP_QUERY_FAILED;
	m_cNetworkManager.SetServerName(svrname);
	m_cNetworkManager.SetWebsite(website);
	//if (visible == 1)
//		m_cNetworkManager.PostMasterlist(true);

	m_cConsole.Init();
	return STARTUP_SUCCESS;
}

void CCore::ReloadGamemode()
{
	// Change mode to our current one -> reload
	this->ChangeModeTo(this->GetGameMode()->GetName());
}


void	CCore::ChangeModeTo(char* newmode)
{

	this->GetLog()->AddNormalLog("Changing mode to: '%s'", newmode);
	// it's neccessary to backup, @newmode might be rewritten during Unloadgamemode()
	char modeToLoad[512];
	strcpy(modeToLoad, newmode);

	// at first tell everybody, that we are reloading mode (make them stop everything)
	RakNet::BitStream bsOutR;
	bsOutR.Write((RakNet::MessageID)ID_SERVERRELOAD);
	this->GetNetworkManager()->GetPeer()->Send(&bsOutR, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);


	// reload gamemode
	this->GetGameMode()->UnloadGameMode();


	// Delete all players
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		this->GetPlayerPool()->Delete(i);
	}

	// Delete all vehicles
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		this->GetVehiclePool()->Delete(i);
	}

	// reset the doors' pool
	this->GetDoorPool()->Reset();

	// reset all files
	this->GetFileTransfer()->Reset();

	// everything is set to default, now load gamemode
	this->GetGameMode()->LoadGameMode(modeToLoad);


	this->GetScripts()->onServerInit();

	// start
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		Slot* slot = this->GetNetworkManager()->GetSlotID(i);
		if (slot->isUsed)
		{
			this->GetFileTransfer()->SendFiles(slot->sa);
		}
	}
}


CNetworkManager*	CCore::GetNetworkManager()
{
	return &m_cNetworkManager;
}
CTickManager*		CCore::GetTickManager()
{
	return &m_cTickManager;
}
CPlayerPool*		CCore::GetPlayerPool()
{
	return &m_cPlayerPool;
}
CVehiclePool*		CCore::GetVehiclePool()
{
	return &m_cVehiclePool;
}

CScripts*			CCore::GetScripts()
{
	return &m_cScripts;
}

CConsole*			CCore::GetConsole()
{
	return &m_cConsole;
}

CGameMode*			CCore::GetGameMode()
{
	return &m_cGameMode;
}
CTimerPool*			CCore::GetTimerPool()
{
	return &m_cTimerPool;
}
CDoorPool*			CCore::GetDoorPool()
{
	return &m_cDoorPool;
}

CPickupPool*			CCore::GetPickupPool()
{
	return &m_cPickupPool;
}

void	CCore::SetDefaultMap(char* map)
{
	sprintf(m_cMapName, map);
}
char*	CCore::GetDefaultMap()
{
	return this->m_cMapName;
}

CLog* CCore::GetLog()
{
	return &this->m_cLog;
}

CFileTransfer*	CCore::GetFileTransfer()
{
	return &this->m_cFileTransfer;
}


CBanSystem*		CCore::GetBanSystem()
{
	return &this->m_cBanSystem;
}

CQueryServer*		CCore::GetQueryServer()
{
	return &this->m_cQueryServer;
}


CMasterList*		CCore::GetMasterServer()
{
	return &this->m_cMaster;
}