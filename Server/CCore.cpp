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
}

// pulse every 1000ms
void CCore::OnSecondElapsed()
{
	m_cNetworkManager.SendPingUpdate();
	m_cPickupPool.Tick();
}
bool CCore::Init(int port,int players, std::string startpos, std::string svrname,std::string mode, int visible)
{
	if (m_cNetworkManager.Init(port, players, startpos, mode) == false)
		return false;
	if (m_cQueryServer.StartServer(port + 1) == false)
		return false;
	m_cNetworkManager.SetServerName(svrname);
	if (visible == 1)
		m_cNetworkManager.PostMasterlist(true);

	m_cConsole.Init();
	return true;
}

void CCore::ReloadGamemode()
{
	// at first tell everybody, that we are reloading mode (make them stop everything)
	RakNet::BitStream bsOutR;
	bsOutR.Write((RakNet::MessageID)ID_SERVERRELOAD);
	this->GetNetworkManager()->GetPeer()->Send(&bsOutR, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	

	char tempname[500];
	strcpy(tempname, this->GetGameMode()->GetName());
	// reload gamemode
	this->GetGameMode()->UnloadGameMode();

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		this->GetPlayerPool()->Delete(i);
	}

	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		this->GetVehiclePool()->Delete(i);
	}

	this->GetGameMode()->LoadGameMode(tempname);


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


void	CCore::ChangeModeTo(char* newmode)
{
	// at first tell everybody, that we are reloading mode (make them stop everything)
	RakNet::BitStream bsOutR;
	bsOutR.Write((RakNet::MessageID)ID_SERVERRELOAD);
	this->GetNetworkManager()->GetPeer()->Send(&bsOutR, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);


	// reload gamemode
	this->GetGameMode()->UnloadGameMode();

	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		this->GetPlayerPool()->Delete(i);
	}

	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		this->GetVehiclePool()->Delete(i);
	}

	this->GetGameMode()->LoadGameMode(newmode);


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
