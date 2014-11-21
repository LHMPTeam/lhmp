#include "CCore.h"
#include "CNetwork.h"
//#include "../shared/tools.h"


CCore::CCore()
{
	pIsRunning			= false;
	pIsGameLoaded		= false;
	m_bIsGameLoaded		= false;
	m_bIsRespawning		= false;

	testStop = true;
//	m_pKeyboard = false;
	//CNetworkManager m_cNetwork;

}

CCore::~CCore()
{
}

// MP init
void CCore::Run()
{
	//MessageBox(NULL,"aha","wtf",MB_OK);
	byte nopEndGame[] = "\x90\x90\x90\x90\x90\x90";
	byte jmp1[] = "\xE9\xE0\x12\x04\x00";			// skip menu - load freeride
	byte cave1[] = "\x60\xA1\x9E\xAF\x63\x00\x83\xF8\x01\x74\x06\x61\xE9\xAC\xEE\xFB\xFF\x61\xE9\x27\xF2\xFB\xFF\x90";
	byte jmp2[] = "\xE9\x46\x11\x04\x00";			// after free ride load, jmp into our code cave
	byte jmp3[]	= "\xE9\x90\x01\x00\x00\x90";
	byte jmp4[] = "\xE9\xFF\x03\x00\x00\x90\x90\x90\x90";
//    byte cave[] = "\xC7\x05\x9E\xAF\x63\x00\x01\x00\x00\x00\xE8\x0F\x5F\xF2\xFF\xE9\xA1\xEE\xFB\xFF"; // writes 1 to 0063AF9E after freeride loads 
	byte cave2[] = "\xC7\x05\x9E\xAF\x63\x00\x01\x00\x00\x00\xE8\x0F\x5F\xF2\xFF\xE9\x1E\xF0\xFB\xFF"; // writes 1 to 0063AF9E after freeride loads - fixed
	byte skipmenu[] = "\xE9\xBA\x0A\x00\x00\x90";
	byte skipmenuNew[] = "\xBE\xB0\x1F\x67\x00\xB8\x10\x00\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90";

	byte nopanic[] = "\xE9\x2A\x01\x00\x00\x90";
	byte noMafiopanic[] = "\xEB\x2A";

	byte disableScriptAddWeapon[] = "\xE9\x30\xAA\x00\x00";//      JMP game.005C76DA  at 005BCCA
	byte disableScriptAddScore[] = "\xE9\x77\x0D\x00\x00";//		JMP 005C76DA at 005C695E   
	
	byte disableMissionObj[] = "\xE9\x63\x67\x00\x00";//      JMP game.005C76DA  at 005BCCA

	byte disableCarBreakScore[] = "\xEB\x0F";
	byte disableScoreFromKilledMafians[] = "\xEB\x14";

	byte carChangePosFix[] = "\xE9\xF1\x00\x00\x00\x90";//   
	byte disableCheats[] = "\xEB\x5F";	// JMP SHORT 0x005F96B0 at 005F964F
	byte disableRadar[] = "\x00";	// 00491D97
	byte disableImmortalModeWhenLockedControls[] = "\xEB\x13";

	DWORD lpflOldProtect;
	// Testing

	/*VirtualProtect((void*)0x005E1175, sizeof(physicsJmp1), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005E1175, (void*)physicsJmp1, sizeof(physicsJmp1));
	VirtualProtect((void*)0x005E1175, sizeof(physicsJmp1), lpflOldProtect, &lpflOldProtect);

	VirtualProtect((void*)0x005E1D9C, sizeof(physicsJmp1), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005E1D9C, (void*)physicsJmp1, sizeof(physicsJmp1));
	VirtualProtect((void*)0x005E1D9C, sizeof(physicsJmp1), lpflOldProtect, &lpflOldProtect);

	*/

	VirtualProtect((void*)0x004CBC1B, sizeof(disableImmortalModeWhenLockedControls), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x004CBC1B, (void*)disableImmortalModeWhenLockedControls, sizeof(disableImmortalModeWhenLockedControls));
	VirtualProtect((void*)0x004CBC1B, sizeof(disableImmortalModeWhenLockedControls), lpflOldProtect, &lpflOldProtect);

	


/*------ Is game loaded -> hook some code in game load, call our naked func IsGameLoaded ---------*/
	// write jmps
	// fix game end - probably

	VirtualProtect((void*)0x005FA171, sizeof(nopEndGame), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005FA171, (void*)nopEndGame, sizeof(nopEndGame));
	VirtualProtect((void*)0x005FA171, sizeof(nopEndGame), lpflOldProtect, &lpflOldProtect);

	/*-----------------	Skip menu, load "Free italy" (jump into our CC)---------------------------*/
	VirtualProtect((void*)0x005F9C4B, sizeof(jmp1), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005F9C4B, (void*)jmp1, sizeof(jmp1));
	VirtualProtect((void*)0x005F9C4B, sizeof(jmp1), lpflOldProtect, &lpflOldProtect);

	/*VirtualProtect((void*)0x005F9BD5, sizeof(jmp4), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005F9BD5, (void*)jmp4, sizeof(jmp4));
	VirtualProtect((void*)0x005F9BD5, sizeof(jmp4), lpflOldProtect, &lpflOldProtect);
	*/
	/*VirtualProtect((void*)0x005F9FD9, sizeof(jmp3), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005F9FD9, (void*)jmp3, sizeof(jmp3));
	VirtualProtect((void*)0x005F9FD9, sizeof(jmp3), lpflOldProtect, &lpflOldProtect);
	*/
	
	/*VirtualProtect((void*)0x005F9E57, sizeof(jmp2), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005F9E57, (void*)jmp2, sizeof(jmp2));
	VirtualProtect((void*)0x005F9E57, sizeof(jmp2), lpflOldProtect, &lpflOldProtect);
	*/
	// write caves

	/*---------------Code cave for our Free italy loading  --------------------------------------*/
	VirtualProtect((void*)0x0063AF30, sizeof(cave1), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x0063AF30, (void*)cave1, sizeof(cave1));
	VirtualProtect((void*)0x0063AF30, sizeof(cave1), lpflOldProtect, &lpflOldProtect);

	/*VirtualProtect((void*)0x0063AFA2, sizeof(cave2), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x0063AFA2, (void*)cave2, sizeof(cave2));
	VirtualProtect((void*)0x0063AFA2, sizeof(cave2), lpflOldProtect, &lpflOldProtect);
	*/
	// protect value addr for writing
	VirtualProtect((void*)0x0063AF9E, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	(*(DWORD*) 0x0063AF9E) = 0;

	/*-------------- Skip profile, game loads the first existing profile or default, it also disables ESC menu ingame */
	
	VirtualProtect((void*)0x00557055, sizeof(skipmenuNew), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x00557055, (void*)skipmenuNew, sizeof(skipmenuNew));
	VirtualProtect((void*)0x00557055, sizeof(skipmenuNew), lpflOldProtect, &lpflOldProtect);
	/*VirtualProtect((void*)0x0056D658, sizeof(skipmenu), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x0056D658, (void*)skipmenu, sizeof(skipmenu));
	VirtualProtect((void*)0x0056D658, sizeof(skipmenu), lpflOldProtect, &lpflOldProtect);*/


	//--------------------------- Pausegame disable
	Tools::Nop(0x1005D1D0, 6);
	Tools::Nop(0x1005D1DD, 7);

	//--------------------------- Skip intro
	Tools::Nop(0x005FB249,4);			// 01
	Tools::Nop(0x005FB2EC,4);			// 02

	//--------------------------- Freeze PED movement / rotation  -- not needed now

	//Tools::Nop(0x004BAA0F,33);			// movement
	
	//test purposes
	//Tools::Nop(0x0044DF12,2);			// rotation 1
	//Tools::Nop(0x0044DF1D,3);			// rotation 2
	//Tools::Nop(0x004BAA58,6);			// animation

	//--------------------------- Game enviroment
	Tools::Nop(0x00560957,2);			// disable traffic/police/ped FIX - (WHEN player doesnt have unlocked Extreme ride)

	Tools::Nop(0x0056090F,17);			// disable traffic, peds, police 
	/*Tools::Nop(0x0056090F,6);			// disable traffic
	Tools::Nop(0x0056091A,6);			// disable police
	*/
	/*---------------------------Ped NO Panic (after shoot)*/
	// PED wouldnt do panic after this
	VirtualProtect((void*)0x004BDE6D, sizeof(nopanic), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x004BDE6D, (void*)nopanic, sizeof(nopanic));
	VirtualProtect((void*)0x004BDE6D, sizeof(nopanic), lpflOldProtect, &lpflOldProtect);

	// PED-gangsters wouldnt do panic after this
	VirtualProtect((void*)0x004BD0F7, sizeof(noMafiopanic), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x004BD0F7, (void*)noMafiopanic, sizeof(noMafiopanic));
	VirtualProtect((void*)0x004BD0F7, sizeof(noMafiopanic), lpflOldProtect, &lpflOldProtect);
	
	// addweapon disabled
	// 005BCCA5   E9 30AA0000      JMP game.005C76DA

	VirtualProtect((void*)0x005BCCA5, sizeof(disableScriptAddWeapon), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005BCCA5, (void*)disableScriptAddWeapon, sizeof(disableScriptAddWeapon));
	VirtualProtect((void*)0x005BCCA5, sizeof(disableScriptAddWeapon), lpflOldProtect, &lpflOldProtect);
	
	// disable mission objectives - titles which appear on map start
	
	VirtualProtect((void*)0x005C0F72, sizeof(disableMissionObj), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005C0F72, (void*)disableMissionObj, sizeof(disableMissionObj));
	VirtualProtect((void*)0x005C0F72, sizeof(disableMissionObj), lpflOldProtect, &lpflOldProtect);

	// fix car change position (on position / rot change)
	/*VirtualProtect((void*)0x0052CD5A, sizeof(carChangePosFix), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x0052CD5A, (void*)carChangePosFix, sizeof(carChangePosFix));
	VirtualProtect((void*)0x0052CD5A, sizeof(carChangePosFix), lpflOldProtect, &lpflOldProtect);
	*/
	// disable cheats (boxer,municak)
	VirtualProtect((void*)0x005F964F, sizeof(disableCheats), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005F964F, (void*)disableCheats, sizeof(disableCheats));
	VirtualProtect((void*)0x005F964F, sizeof(disableCheats), lpflOldProtect, &lpflOldProtect);

	// disable radar (in car)
	
	VirtualProtect((void*)0x00491D9B, sizeof(disableRadar), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x00491D9B, (void*)disableRadar, sizeof(disableRadar));
	VirtualProtect((void*)0x00491D9B, sizeof(disableRadar), lpflOldProtect, &lpflOldProtect);

	// disable scripts

	/*VirtualProtect((void*)0x005AF836, sizeof(disableScripts), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005AF836, (void*)disableScripts, sizeof(disableScripts));
	VirtualProtect((void*)0x005AF836, sizeof(disableScripts), lpflOldProtect, &lpflOldProtect);
	*/

	//Tools::WriteBytes(0x00491D97, disableRadar, sizeof(disableRadar));

	// disable AddScore 
	VirtualProtect((void*)0x005C695E, sizeof(disableScriptAddScore), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x005C695E, (void*)disableScriptAddScore, sizeof(disableScriptAddScore));
	VirtualProtect((void*)0x005C695E, sizeof(disableScriptAddScore), lpflOldProtect, &lpflOldProtect);

	//disable Adding score from breaked cars 
	VirtualProtect((void*)0x00468D73, sizeof(disableCarBreakScore), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x00468D73, (void*)disableCarBreakScore, sizeof(disableCarBreakScore));
	VirtualProtect((void*)0x00468D73, sizeof(disableCarBreakScore), lpflOldProtect, &lpflOldProtect);

	//disable Adding score from Mafians
	VirtualProtect((void*)0x00497465, sizeof(disableScoreFromKilledMafians), PAGE_EXECUTE_READWRITE, &lpflOldProtect);
	memcpy((void*)0x00497465, (void*)disableScoreFromKilledMafians, sizeof(disableScoreFromKilledMafians));
	VirtualProtect((void*)0x00497465, sizeof(disableScoreFromKilledMafians), lpflOldProtect, &lpflOldProtect);


	//-------------------------	Disable damage from collision
	Tools::Nop(0x00518328, 23);
	Tools::Nop(0x00534C51, 28);
	Tools::Nop(0x00536723, 23);
	Tools::Nop(0x00536BA1, 23);

	//Tools::Nop(0x0046B566, 65);
	
	//-------------------------

}

void CCore::Start()
{
	m_cLocalPlayer.Init();
	m_cNetwork.ConnectServer();
	m_crashhandler.Prepare();
	this->GetGame()->DisableBridges();
}
void CCore::Pulse()
{
	if (m_bIsRespawning == false)
	{
		m_cLocalPlayer.Pulse();
	}
	m_cNetwork.Pulse();
	
}
void CCore::PulseAfterSec()
{
	
}
bool CCore::IsRunning()
{
	return pIsRunning;
}

void CCore::SetRunning(bool b)
{
	pIsRunning = 1;
}

bool CCore::IsLoaded()
{
	return pIsGameLoaded;
}

void CCore::SetLoaded(bool b)
{
	pIsGameLoaded = 1;
}

void CCore::OnLostConnection()
{
	// should reset all modules to init & call reconnect

}
void CCore::Render(IDirect3DDevice8* pInterface,LPD3DXFONT font)
{
	m_cChat.Render(pInterface,font);
	//MessageBox(NULL,"Funguje","j",MB_OK);
}

void CCore::D3DInit(IDirect3DDevice8* d3dDevice)
{
	//m_cChat.ChatInit(d3dDevice);
	//LPCSTR	message	= "TestMessage";
	m_cChat.AddMessage("Started Lost Heaven Multiplayer[TEST]");
	//MessageBox(NULL,"D3D init","lol",MB_OK);
}

CChat*	CCore::GetChat()
{
	return &m_cChat;
}

CNetworkManager*	CCore::GetNetwork()
{
	return &m_cNetwork;
}

CLocalPlayer*		CCore::GetLocalPlayer()
{
	return &m_cLocalPlayer;
}
CGame*		CCore::GetGame()
{
	return &m_cGame;
}
CGraphics*		CCore::GetGraphics()
{
	return &m_cGraphics;
}
/*CPlayerSync*		CCore::GetPlayerSync()
{
	return &m_cPlayerSync;
}*/

CEngineStack*		CCore::GetEngineStack()
{
	return &m_cEngineStack;
}

CPedPool*			CCore::GetPedPool()
{
	return &m_cPedPool;
}
CLog*				CCore::GetLog()
{
	return &m_cLog;
}

CVehiclePool*	CCore::GetVehiclePool()
{
	return &m_cVehPool;
}


CCrashHandler*	CCore::GetCrashHandler()
{
	return &this->m_crashhandler;
}

CKeyboard*	CCore::GetKeyboard()
{
	return &this->m_cKeyboard;
}

CIngameMenu* CCore::GetIngameMenu()
{
	return &this->m_cIngameMenu;
}

CPickupPool*	CCore::GetPickupPool()
{
	return &m_cPickuppool;
}
