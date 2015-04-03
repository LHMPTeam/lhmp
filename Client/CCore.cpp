#include "CCore.h"
#include "CNetwork.h"
//#include "../shared/tools.h"


CCore::CCore()
{
	pIsRunning			= false;
	pIsGameLoaded		= false;
	m_bIsGameLoaded		= false;
	m_bIsRespawning		= false;

	testStop = false;
//	m_pKeyboard = false;
	//CNetworkManager m_cNetwork;

}

CCore::~CCore()
{
}

// MP init
void CCore::Run()
{
	byte nopEndGame[] = "\x90\x90\x90\x90\x90\x90";
	
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
	//byte disableRadar[] = "\x00";	// 00491D97
	byte disableImmortalModeWhenLockedControls[] = "\xEB\x13";

	byte jmpToFreeride[] = "\xE9\x9D\x01\x00\x00";		// JMP Game.005F9DED
	byte disableESCmenu[] = "\xE9\xA5\x00\x00\x00\x00";	// JMP Game.005F97B3

	byte drivebyshootfix[] = "\x3E\xD9\x5C\x24\x40\x90";



	PatchBytes(0x004CBC1B, disableImmortalModeWhenLockedControls);

/*------ Is game loaded -> hook some code in game load, call our naked func IsGameLoaded ---------*/
	// fix game end - probably
	// TODO: seems to be useless as we have disabled ESC menu and any way to 
	//PatchBytes(0x005FA171, nopEndGame);

	/*----------------- Prevent ESC menu trigger                      ----------------------------*/
	PatchBytes(0x005F9709, disableESCmenu);

	/*-----------------	Skip menu, load "Free italy" (jump into our CC)---------------------------*/
	PatchBytes(0x005F9C4B, jmpToFreeride);

	/*-------------- Skip profile, game loads the first existing profile or default*/
	// NOTE: 0056CCC0 is function called everytime the menu open action is triggered
	PatchBytes(0x00557055, skipmenuNew);

	//--------------------------- Pausegame disable
	//******************************
	// Test if it changes something, looks useless
	Tools::Nop(0x1005D1D0, 6);
	Tools::Nop(0x1005D1DD, 7);
	//******************************

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
	PatchBytes(0x004BDE6D, nopanic);

	// PED-gangsters wouldnt do panic after this
	PatchBytes(0x004BD0F7, noMafiopanic);
	
	// addweapon script command disabled
	PatchBytes(0x005BCCA5, disableScriptAddWeapon);
	
	// disable mission objectives - titles which appear on map start
	PatchBytes(0x005C0F72, disableMissionObj);

	//******************************************************************************************************************
	// fix car change position (on position / rot change)
	// NOTE: thanks to this game recalculates car's physic when car has a driver, but standing at one place
	PatchBytes(0x0052CD5A, carChangePosFix);
	
	//******************************************************************************************************************
	// disable cheats (boxer,municak)
	PatchBytes(0x005F964F, disableCheats);

	// TODO - what next ?
	// disable radar (in car)
	//PatchBytes(0x00491D9B, disableRadar);

	// disable scripts
	//PatchBytes(0x005AF836, disableScripts);

	// disable AddScore 
	PatchBytes(0x005C695E, disableScriptAddScore);

	//disable Adding score from breaked cars 
	PatchBytes(0x00468D73, disableCarBreakScore);

	//disable Adding score from Mafians
	PatchBytes(0x00497465, disableScoreFromKilledMafians);


	//-------------------------	Disable damage from collision
	Tools::Nop(0x00518328, 23);
	Tools::Nop(0x00534C51, 28);
	Tools::Nop(0x00536723, 23);
	Tools::Nop(0x00536BA1, 23);
	//-------------------------

	// Fix in-car remote player weapon rotation (drive-by shooting)
	//PatchBytes(0x0049BB23, drivebyshootfix);
	// now as a hook (check CHooks)

}

void CCore::ShutdownClient()
{
	this->GetNetwork()->GetPeer()->Shutdown(100, 0, IMMEDIATE_PRIORITY);
	TerminateProcess(GetCurrentProcess(), 0);
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

CFileTransfer*	CCore::GetFileTransfer()
{
	return &this->m_cFileTransfer;
}

CFileSystem*	CCore::GetFileSystem()
{
	return &this->m_cFileSystem;
}


CNametags*		CCore::GetNametags()
{
	return &this->m_cNametags;
}