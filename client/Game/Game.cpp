#define MAFIA_SDK_IMPLEMENTATION
#include <stdinc.h>

Game::Game()
	: mLocalPlayer(nullptr), 
	mTickManager(new TickManager())
{
}

Game::~Game()
{
}

int counter = 0;
void Game::Init()
{
	// Disable logos
	MemoryPatcher::InstallNopPatch(0x5BFDDC, 60);

	// Disable - Please Wait text
	MemoryPatcher::InstallNopPatch(0x60D391, 15);

	// 0x5BEDC2 - hInstance of GM_MainMenu ?
	// Disable - GM_Menu::Create
	MemoryPatcher::PatchAddress(0x5EA2E0, 0xC3);

	// Skip to the freeride
	MemoryPatcher::PatchAddress(0x5BF3D8, 4231955727); // skip to 0x5BF01C 
	MemoryPatcher::PatchAddress(0x5BEE7A, 105961); // E9 9D0100000
	
	// Never loading the FreeRide setup player
	MemoryPatcher::InstallNopPatch(0x5BF06E, 5);

	// Disable - game menu
	BYTE disableGameMenu[] = "\xE9\xAB\x00";
	MemoryPatcher::PatchAddress(0x005BE919, disableGameMenu, sizeof(disableGameMenu));

	// Disable - inventory
	BYTE disableInventory[] = "\xEB\x15";
	MemoryPatcher::PatchAddress(0x00592001, disableInventory, sizeof(disableInventory));

	// Disable - inventory in vehicle
	BYTE disableInventoryCar[] = "\xEB\x0F";
	MemoryPatcher::PatchAddress(0x00590139, disableInventoryCar, sizeof(disableInventoryCar));

	// Disable - suspended process
	BYTE disableProcess[] = "\xEB\x69";
	MemoryPatcher::PatchAddress(0x1006DAB7, disableProcess, sizeof(disableProcess));

	// Disable - mafia scripts
	BYTE disableScripts[] = "\xEB\x3F";
	MemoryPatcher::PatchAddress(0x00461636, disableScripts, sizeof(disableScripts));

	// Respawn - no black screen ( its bullshet it  nops zatmyse)
	//BYTE respawnPlayer[] = "\xC2\x0C\x00\x90";
	//MemoryPatcher::PatchAddress( 0x005FA370, respawnPlayer, sizeof( respawnPlayer ) );

	//BYTE respawnPlayer2[] = "\xE9\xAD\x82\x00\x00";
	//MemoryPatcher::PatchAddress(0x004716ED, respawnPlayer2, sizeof(respawnPlayer2));

	// Water sink respawn
	BYTE respawnWater[] = "\xE9\xC2\x00\x00\x00";
	MemoryPatcher::PatchAddress(0x005A5290, respawnWater, sizeof(respawnWater));

	//No change model when blow in car
	MemoryPatcher::InstallNopPatch(0x0058A5DA, 14);

	//Respwn no camera unbind meybe (TODO)
	//No delete actor and anims
	BYTE noDeleteActors[] = "\xC7\x86\x0C\x04\x00\x00";
	MemoryPatcher::InstallNopPatch(0x00572F3B, 10);
	MemoryPatcher::InstallNopPatch(0x0057838F, 10);
	MemoryPatcher::InstallNopPatch(0x0057AB8C, 10);
	MemoryPatcher::InstallNopPatch(0x0057ACD1, 10);
	MemoryPatcher::InstallNopPatch(0x0058A780, 10);

	//Setup hooks
	MafiaSDK::C_Game_Hooks::HookOnGameInit([&]() {

		if(++counter == 2)
			this->mShouldStart = true;
	});

	MafiaSDK::C_Game_Hooks::HookOnGameTick([&]() {

		if (Core::GetCore()->IsRunning())
		{
			Core::GetCore()->Tick();
		}
	});
}

void Game::OnGameStart()
{
	Core::GetCore()->GetGraphics()->GetLoadingScreen()->SetLoading(false);
	MafiaSDK::GetMission()->GetGame()->SetTrafficVisible(false);
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Mission Loaded Bitch", 0xFF0000);
	Core::GetCore()->GetNetwork()->Init();
	//Core::GetCore()->GetNetwork()->Connect("84.16.39.2", 27015);
}

void Game::OnGameInit()
{
	
}

void Game::Tick()
{
	if (mShouldStart)
	{
		if (MafiaSDK::GetMission()->GetGame() != nullptr)
		{
			OnGameStart();
		}

		mShouldStart = false;
	}

	mTickManager->GameTick();

	//Update Game Objects 
	for (auto player : Core::GetCore()->GetNetwork()->GetPlayers())
	{
		player.second->UpdateGameObject();
	}
}
