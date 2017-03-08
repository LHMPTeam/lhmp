#define MAFIA_SDK_IMPLEMENTATION
#include <stdinc.h>

Game::Game()
	: mLocalPlayer(nullptr), 
	mConnectingCameraNumber(0),
	mCameraTimer(RakNet::GetTimeMS()),
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
	//MemoryPatcher::InstallNopPatch(0x60D391, 15);

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

	BYTE respawnPlayer2[] = "\xE9\xAD\x82\x00\x00";
	MemoryPatcher::PatchAddress(0x004716ED, respawnPlayer2, sizeof(respawnPlayer2));

	// Water sink respawn
	BYTE respawnWater[] = "\xE9\xC2\x00\x00\x00";
	MemoryPatcher::PatchAddress(0x005A5290, respawnWater, sizeof(respawnWater));

	BYTE noEndMissionJmp[] = "\xE9\xAD\x82\x00";
	MemoryPatcher::PatchAddress(0x004716ED, noEndMissionJmp, sizeof(noEndMissionJmp));

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

		if (++counter == 2)
			this->mShouldStart = true;
	});

	MafiaSDK::C_Game_Hooks::HookOnGameTick([&]() {

		if (Core::GetCore()->IsRunning())
		{
			Core::GetCore()->Tick();
		}
	});

	MafiaSDK::C_Human_Hooks::HookOnHumanHit([&](MafiaSDK::C_Human* thisInstance, int hitType, const Vector3D & unk1, const Vector3D & unk2, const Vector3D & unk3, float damage, MafiaSDK::C_Actor* atacker, unsigned long hittedPart, MafiaSDK::I3D_Frame* targetFrame) {

		//If we are target damage done is sended to server and broadcasted later
		if (Core::GetCore()->GetCore()->GetNetwork()->IsConnected())
		{
			if (thisInstance == MafiaSDK::GetMission()->GetGame()->GetLocalPlayer())
			{
				damage = 20.0f;

				//Get Atacker
				auto atackerGUID = Core::GetCore()->GetNetwork()->GetPlayerGUIDByActor(thisInstance);
				
				RakNet::BitStream bitStream;
				//Lets send all sheet from hit 
				bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
				bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_ONHIT));
				bitStream.Write(hitType);
				bitStream.Write(unk1);
				bitStream.Write(unk2);
				bitStream.Write(unk3);
				bitStream.Write(damage);
				bitStream.Write(atackerGUID);
				bitStream.Write(hittedPart);
				Core::GetCore()->GetNetwork()->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Core::GetCore()->GetNetwork()->GetServerAddress(), false);

				//Check if im dead if yep start respawning 
				bool isAlreadyDead = thisInstance->GetInterface()->entity.isActive == 0;

				//Call and do damage to me 
				thisInstance->Hit(hitType, unk1, unk2, unk3, damage, atacker, hittedPart, NULL);

				if (!isAlreadyDead)
				{
					//Player is dead now !
					if (!thisInstance->GetInterface()->entity.isActive)
					{
						//Respawn it now !
						Core::GetCore()->GetGame()->GetLocalPlayer()->Respawn();
					}
				}
			}
		}
			
		//If ped is damaged ignore hits 
		//Server will send proper hits 

		return 0;
	});

	MafiaSDK::C_Game_Hooks::HookLocalPlayerFallDown([&]() {
	
		if (!Core::GetCore()->GetGame()->GetLocalPlayer()->IsRespawning())
			Core::GetCore()->GetGame()->GetLocalPlayer()->Respawn();
	});

	MafiaSDK::C_Human_Hooks::HookOnHumanShoot([&](const Vector3D & position) {

		if (Core::GetCore()->GetCore()->GetNetwork()->IsConnected())
		{
			RakNet::BitStream bitStream;
			//Lets send all sheet from hit 
			bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
			bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_ONSHOOT));
			bitStream.Write(position);
			Core::GetCore()->GetNetwork()->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Core::GetCore()->GetNetwork()->GetServerAddress(), false);
		}
	});

	MafiaSDK::C_Human_Hooks::HookHumanDoWeaponChange([&](MafiaSDK::C_Human* thisInstance, byte weaponId) {
	
		if (thisInstance == MafiaSDK::GetMission()->GetGame()->GetLocalPlayer())
		{
			RakNet::BitStream bitStream;
			
			bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER));
			bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMPID_PLAYER_WEAPON_SWITCH));
			bitStream.Write(weaponId);
			Core::GetCore()->GetNetwork()->GetPeer()->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, Core::GetCore()->GetNetwork()->GetServerAddress(), false);
		}
	});
}

void Game::OnGameStart()
{
	MafiaSDK::GetMission()->GetGame()->SetTrafficVisible(false);
	Core::GetCore()->GetGraphics()->GetLoadingScreen()->SetLoading(false);
	UpdateConnectingCamera();

	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"{FFCC2002}[LHMP] {FFf9f8f7} Initialzed (Pre Alpha 0.1) !");
	Core::GetCore()->GetGraphics()->GetChat()->AddMessage(L"{FFCC2002}[LHMP] {FFf9f8f7} Type /connect <IP> For joining server !");
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

	if (!Core::GetCore()->GetNetwork()->IsConnected())
	{
		if (RakNet::GetTimeMS() - mCameraTimer > 5000)
		{
			UpdateConnectingCamera();
			mCameraTimer = RakNet::GetTimeMS();
		}
	}

	//Update Game Objects 
	for (auto player : Core::GetCore()->GetNetwork()->GetPlayers())
	{
		player.second->UpdateGameObject();
	}
}

void Game::UpdateConnectingCamera()
{
	if (mConnectingCameraNumber >= mConnectingCameras.size())
		mConnectingCameraNumber = 0;

	MafiaSDK::GetMission()->GetGame()->GetCamera()->LockAt(mConnectingCameras.at(mConnectingCameraNumber).first, mConnectingCameras.at(mConnectingCameraNumber).second);

	mConnectingCameraNumber++;
}
