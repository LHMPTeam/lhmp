#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include "CNetwork.h"
#include "CLocalPlayer.h"
#include <d3d8.h>				
#include "CChat.h"
#include "CGame.h"
#include "CGraphics.h"
//#include "CPlayerSync.h"
#include "CEngineStack.h"
#include "CPedPool.h"
#include "CVehiclePool.h"
#include "CLog.h"
#include "CCrashHandler.h"
#include "CKeyboard.h"
#include "CIngameMenu.h"
#include "CColoredText.h"
#include <time.h>

#include "../shared/tools.h"


class CCore
{
private:
	bool					pIsRunning;
	bool					pIsGameLoaded;
	CNetworkManager			m_cNetwork;
	CLocalPlayer			m_cLocalPlayer;
	CChat					m_cChat;
	CGame					m_cGame;
	CGraphics				m_cGraphics;
	//CPlayerSync				m_cPlayerSync;
	CEngineStack			m_cEngineStack;
	CPedPool				m_cPedPool;
	CVehiclePool			m_cVehPool;
	CLog					m_cLog;
	CCrashHandler			m_crashhandler;
	CKeyboard				m_cKeyboard;
	CIngameMenu				m_cIngameMenu;


public:
	CCore();
	~CCore();
	void					Run();
	void					Start();
	void					Pulse();
	bool					IsRunning();
	void					SetRunning(bool);
	bool					IsLoaded();
	void					SetLoaded(bool);
	void					Render(IDirect3DDevice8*,LPD3DXFONT);
	void					D3DInit(IDirect3DDevice8*);
	void					OnLostConnection();
	CChat*					GetChat();
	CNetworkManager*		GetNetwork();
	CLocalPlayer*			GetLocalPlayer();
	CGame*					GetGame();
	CGraphics*				GetGraphics();
	//CPlayerSync*			GetPlayerSync();
	CEngineStack*			GetEngineStack();
	CPedPool*				GetPedPool();
	CVehiclePool*			GetVehiclePool();
	CLog*					GetLog();
	CCrashHandler*			GetCrashHandler();
	CKeyboard*				GetKeyboard();
	CIngameMenu*			GetIngameMenu();

	bool					m_bIsGameLoaded;
	bool					m_bIsRespawning;         

	bool					testStop;

};



#endif