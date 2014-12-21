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
	m_cNetworkManager.m_pSvrName = svrname;
	if (visible == 1)
		m_cNetworkManager.PostMasterlist(true);

	m_cConsole.Init();
	return true;
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

CFileSystem* CCore::GetFileSystem()
{
	return &this->m_cFileSystem;
}

CFileTransfer*	CCore::GetFileTransfer()
{
	return &this->m_cFileTransfer;
}