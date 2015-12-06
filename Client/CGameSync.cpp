#include "CGameSync.h"
#include "CCore.h"
#include "../shared/CBitArray.h"

extern CCore* g_CCore;

// When local player picks up a weapon from the ground
void CGameSync::Engine_onPlayerTakeWeapon(DWORD wepId, DWORD wepLoaded, DWORD wepHidden)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_ADDWEAPON);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	bsOut.Write(wepId);
	bsOut.Write(wepLoaded);
	bsOut.Write(wepHidden);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When local player swap his weapon 
void CGameSync::Engine_onChangeWeapon(DWORD wepId)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SWITCHWEAPON);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	bsOut.Write(wepId);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When local player shoot
void CGameSync::Engine_onShoot(Vector3D position, DWORD weaponID)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SHOOT);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	bsOut.Write(position);
	bsOut.Write(weaponID);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When local player is shot by another player
void CGameSync::Engine_onPlayerGetHit(CPed* attacker)
{
	int playerID = 0;
	for (playerID = 0; playerID < MAX_PLAYERS; playerID++)
		if (g_CCore->GetPedPool()->Return(playerID) == attacker)
			break;

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_HIT);
	bsOut.Write(playerID);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When local player is killed by enemy
void CGameSync::Engine_onPlayerDie(CPed* enemy, unsigned char hitbox)
{
	int playerID = 0;
	for (playerID = 0; playerID < MAX_PLAYERS; playerID++)
		if (g_CCore->GetPedPool()->Return(playerID) == enemy)
			break;

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_DEATH);
	bsOut.Write(playerID);
	bsOut.Write(hitbox);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When local player enters a car
void CGameSync::Engine_onPlayerEnterVehicle(CVehicle* vehicle, DWORD seatID)
{
	int vehID = 0;
	for (vehID = 0; vehID < MAX_PLAYERS; vehID++)
		if (g_CCore->GetVehiclePool()->Return(vehID) == vehicle)
			break;
	if (vehID != MAX_VEHICLES)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_PLAYER_ENTERED_VEHICLE);
		bsOut.Write(vehID);
		bsOut.Write(seatID);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
	}
	else {
		// TODO: report error
	}
}
// When local player exits a car
void CGameSync::Engine_onPlayerExitVehicle(CVehicle* vehicle)
{
	int vehID = 0;
	for (vehID = 0; vehID < MAX_PLAYERS; vehID++)
		if (g_CCore->GetVehiclePool()->Return(vehID) == vehicle)
			break;
	if (vehID != MAX_VEHICLES)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_PLAYER_EXIT_VEHICLE);
		bsOut.Write(vehID);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
	} else {
		// TODO: report error
	}
}
// When local player jacks someone from a car
void CGameSync::Engine_onPlayerCarjack(CVehicle* vehicle, int seatId)
{
	int vehID = 0;
	for (vehID = 0; vehID < MAX_PLAYERS; vehID++)
		if (g_CCore->GetVehiclePool()->Return(vehID) == vehicle)
			break;
	if (vehID != MAX_VEHICLES)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_JACK);
		bsOut.Write(vehID);
		bsOut.Write(seatId);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
	}
	else {
		// TODO: report error
	}
}
// When local player throws a granade/molotov
void CGameSync::Engine_onPlayerThrowGranade(Vector3D position)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_THROWGRANADE);
	bsOut.Write(position);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When local player throws a granade/molotov
void CGameSync::Engine_onDoorStateChange(char* doorName, int state, bool facing)
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_DOOR_SET_STATE);
	bsOut.Write(doorName);
	bsOut.Write(state);
	bsOut.Write(facing);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When a car explodes
void CGameSync::Engine_onExplodeCar(CVehicle* vehicle)
{
	int vehID = 0;
	for (vehID = 0; vehID < MAX_PLAYERS; vehID++)
		if (g_CCore->GetVehiclePool()->Return(vehID) == vehicle)
			break;
	if (vehID != MAX_VEHICLES)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_ON_EXPLODED);
		bsOut.Write(vehID);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
	}
	else {
		// report error
	}
}
// When player died and the game is about to reload
void CGameSync::Engine_onMapRespawn()
{
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_DEATH_END);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}
// When vehicle's engine changes
void CGameSync::Engine_onVehicleEngineStateChange(CVehicle* vehicle, BYTE state)
{
	vehicle->ToggleEngine(state);
	int vehID = 0;
	for (vehID = 0; vehID < MAX_PLAYERS; vehID++)
		if (g_CCore->GetVehiclePool()->Return(vehID) == vehicle)
			break;
	if (vehID != MAX_VEHICLES)
	{
		RakNet::BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_TOGGLE_ENGINE);
		bsOut.Write(vehID);
		bsOut.Write(state);
		g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
	}
	else {
		//report an error
	}
}


/******************************************************************************
							Server side events 
*******************************************************************************/

void CGameSync::onTextMessage(RakNet::BitStream* bitInput)
{
	unsigned short messageLen;
	// the lenght of message might vary
	bitInput->Read(messageLen);
	// allocate enough memory for message
	char* buff = new char[messageLen + 100];
	bitInput->Read(buff);
	// add new message into message buffer
	g_CCore->GetChat()->AddMessage(buff);
	// free the memory
	delete[] buff;
}

//TODO: skip health in regular updates, sync it via hit events
void CGameSync::onPlayerUpdateOnFoot(RakNet::BitStream* bitInput,RakNet::TimeMS timestamp)
{
	SYNC::ON_FOOT_SYNC syncData;
	bitInput->Read(syncData);
	// if packet have valid player ID
	if (syncData.ID >= 0 && syncData.ID <= MAX_PLAYERS)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(syncData.ID);
		// if there is a ped with such an ID
		if (ped)
		{
			// if the packet is not outdated
			if (timestamp - ped->GetTimeStamp() > 0)
			{
				// apply packet data
				Vector3D rot = Tools::ComputeOffsetDegrees(syncData.rotation*360.0f / MAX_USHORT);
				ped->SetRotation(rot);
				ped->SetState(syncData.animStatus);
				ped->SetHealth((float)syncData.health);

				CBitArray states(syncData.states);

				ped->SetDucking(states.GetBit(ONFOOT_ISDUCKING));
				ped->SetAiming(states.GetBit(ONFOOT_ISAIMING));
				ped->SetCarAnim(states.GetBit(ONFOOT_ISCARANIM));
				ped->SetTimeStamp(timestamp);
				ped->SetPosition(syncData.position);
				ped->SetUpInterpolation();
				ped->UpdateGameObject();
			}
		}
	}
}

// TODO: use CGame structs to apply values instead of direct memory access
void CGameSync::onPlayerUpdateInCar(RakNet::BitStream* bitInput)
{
	SYNC::IN_CAR syncData;
	bitInput->Read(syncData);
	//if player ID is of range <0,MAXPLAYERS>
	if (syncData.ID >= 0 && syncData.ID <= MAX_PLAYERS)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(syncData.ID);
		// if there is a PED with such ID
		if (ped)
		{
			ped->SetHealth(syncData.health);
			ped->SetAiming(syncData.isAiming);
			if (ped->GetEntity() != NULL)
			{
				//TODO: use structs
				*(float*)(ped->GetEntity() + 0x5F4) = syncData.aim;
				*(float*)(ped->GetEntity() + 0x644) = syncData.health;

				if (ped->GetCurrentWeapon() != 0)
					*(byte*)(ped->GetEntity() + 0x1E5) = syncData.isAiming;
				else
					*(byte*)(ped->GetEntity() + 0x1E5) = 0;
			}
		}
	}
}


void CGameSync::onPlayerConnect(RakNet::BitStream* bitInput)
{
	char sz_nickname[255];
	int PlayerID, skinId;
	unsigned int color;

	bitInput->Read(PlayerID);
	bitInput->Read(sz_nickname); // nickname
	bitInput->Read(skinId); 
	bitInput->Read(color);

	// creates new instance
	g_CCore->GetPedPool()->New(PlayerID);
	g_CCore->GetLog()->AddLog("LHMP_CONNECTORCREATEPED");
	CPed* ped = g_CCore->GetPedPool()->Return(PlayerID);
	if (ped != 0)
	{
		ped->SetName(sz_nickname);
		ped->SetSkinId(skinId);
		ped->SetNickColor(color);
	}
	else {
		// TODO: report error (couldn't create a new instace - WTF)
	}
}

// TODO: replace all InCar with member get-func
void CGameSync::onPlayerDisconnect(RakNet::BitStream* bitInput)
{
	int ID;
	bitInput->Read(ID);
	CPed* ped = g_CCore->GetPedPool()->Return(ID);
	if (ped)
	{
		if (ped->InCar != -1)
		{
			CVehicle* veh = g_CCore->GetVehiclePool()->Return(ped->InCar);
			if (veh)
			{
				// TODO: create func onPlayerDisconnect at Vehicle
				veh->SetSpeed(Vector3D(0.0f));
			}
		}
		if (ped->GetEntity() != NULL)
		{
			g_CCore->GetEngineStack()->AddMessage(ES_DELETEPLAYER, ped->GetEntity());
		}
		g_CCore->GetPedPool()->Delete(ID);
	}
}

// when player changes skin
void CGameSync::onPlayerChangeSkin(RakNet::BitStream* bitInput)
{
	int ID, skinID;
	bitInput->Read(ID);
	bitInput->Read(skinID);
	if (ID == g_CCore->GetLocalPlayer()->GetOurID())
	{
		g_CCore->GetLocalPlayer()->ourSkin = skinID;
		g_CCore->GetEngineStack()->AddMessage(ES_CHANGESKIN, ID);
	}
	else
	{
		CPed* ped = g_CCore->GetPedPool()->Return(ID);
		if (ped == 0)
		{
			ped->SetSkinId(skinID);
			g_CCore->GetEngineStack()->AddMessage(ES_CHANGESKIN, ID);
		}
	}
}

void CGameSync::onPlayerPlayAnimation(RakNet::BitStream* bitInput)
{
	int ID;
	char animString[255];
	bitInput->Read(ID);
	bitInput->Read(animString);

	// append a new messsage about animation
	ENGINE_STACK::PLAYER_PLAYANIM* pw = new ENGINE_STACK::PLAYER_PLAYANIM(ID, animString);
	g_CCore->GetEngineStack()->AddMessage(ES_PLAYANIM_STRING, (DWORD)pw);
}

void CGameSync::onPlayerRespawn(RakNet::BitStream* bitInput)
{
		int ID;
		bitInput->Read(ID);
		CPed* ped = g_CCore->GetPedPool()->Return(ID);
		if (ped)
		{
			// TODO: incar
			if (ped->InCar != -1)
			{
				CVehicle* veh = g_CCore->GetVehiclePool()->Return(ped->InCar);
				if (veh != NULL)
				{
					for (int i = 0; i < 4; i++)
					{
						if (veh->GetSeat(i) == ID)
							veh->SetSeat(i, 0);
					}
				}
				// TODO incar set
				ped->InCar = -1;
				veh->PlayerExit(ID);
				ped->SetIsOnFoot(true);
			}
			ped->ClearWeapons();
			g_CCore->GetEngineStack()->AddMessage(ES_CREATEPLAYER, ID);
		}
}

void CGameSync::playSound(RakNet::BitStream* bitInput)
{
		// WARNING: theoretically, dangerous (buffer overflow)
		char sound[255];
		bitInput->Read(sound);
		ENGINE_STACK::PLAYER_PLAYSOUND* pw = new ENGINE_STACK::PLAYER_PLAYSOUND(sound);
		g_CCore->GetEngineStack()->AddMessage(ES_PLAYSOUND_STRING, (DWORD)pw);
}


void CGameSync::onPlayerPingArrives(RakNet::BitStream* bitInput)
{
		int ID, ping;
		bitInput->Read(ID);
		bitInput->Read(ping);
		if (ID == g_CCore->GetLocalPlayer()->GetOurID())
		{
			g_CCore->GetLocalPlayer()->SetPing(ping);
		}
		else
		{
			CPed* ped = g_CCore->GetPedPool()->Return(ID);
			if (ped)
			{
				ped->SetPing(ping);
			}
		}
}


void CGameSync::onPlayerAddWeapon(RakNet::BitStream* bitInput)
{
	int ID, wepID, wepLoaded, wepHidden;
	bitInput->Read(ID);
	bitInput->Read(wepID);
	bitInput->Read(wepLoaded);
	bitInput->Read(wepHidden);
	ENGINE_STACK::PLAYER_ADDWEAPON* pw = new ENGINE_STACK::PLAYER_ADDWEAPON(ID, wepID, wepLoaded, wepHidden);
	if (g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
		if (ped != NULL)
		{
			ped->AddWeapon(pw->wepID, pw->wepLoaded, pw->wepHidden);
			if (ped->GetEntity() == NULL)
			{
				return;
			}
		}

	}
	g_CCore->GetEngineStack()->AddMessage(ES_ADDWEAPON, (DWORD)pw);
	char buff[255];
	sprintf(buff, "[NM]AddWep: %i %i %i", pw->wepID, pw->wepLoaded, pw->wepHidden);
	g_CCore->GetLog()->AddLog(buff);
}

// when player is taken the weapon
void CGameSync::onPlayerDeleteWeapon(RakNet::BitStream* bitInput)
{
	int ID, wepID;
	bitInput->Read(ID);
	bitInput->Read(wepID);
	ENGINE_STACK::PLAYER_DELETEWEAPON* pw = new ENGINE_STACK::PLAYER_DELETEWEAPON(ID, wepID);
	if (g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
		if (ped != NULL)
			ped->DeleteWeapon(pw->wepID);
	}
	g_CCore->GetEngineStack()->AddMessage(ES_DELETEWEAPON, (DWORD)pw);
	g_CCore->GetLog()->AddLog("[NM] delwep");
}


void CGameSync::onPlayerChangeWeapon(RakNet::BitStream* bitInput)
{
	int ID, wepID;
	bitInput->Read(ID);
	bitInput->Read(wepID);
	ENGINE_STACK::PLAYER_SWITCHWEAPON* pw = new ENGINE_STACK::PLAYER_SWITCHWEAPON(ID, wepID);
	if (g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
		if (ped != NULL)
			ped->SwitchWeapon(pw->wepID);
	}
	else {
		g_CCore->GetEngineStack()->AddMessage(ES_SWITCHWEAPON, (DWORD)pw);
		g_CCore->GetLog()->AddLog("[NM]switchwep");
	}
}

void CGameSync::onPlayerShoot(RakNet::BitStream* bitInput)
{
	int ID, currentID;
	Vector3D pos;
	bitInput->Read(ID);
	bitInput->Read(pos);
	bitInput->Read(currentID);
	ENGINE_STACK::PLAYER_SHOOT* pw = new ENGINE_STACK::PLAYER_SHOOT(ID, pos);
	char buff[255];
	sprintf(buff, "[NM] Shot %i %f %f %f %d", pw->ID, pw->pos.x, pw->pos.y, pw->pos.z, currentID);
	if (g_CCore->GetLocalPlayer()->GetOurID() != pw->ID)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(pw->ID);
		if (ped != NULL)
		{
			ped->SetCurrentWeapon(currentID);
			ped->OnShoot();
		}
	}
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetEngineStack()->AddMessage(ES_SHOOT, (DWORD)pw);
}


void CGameSync::onPlayerThrowGranade(RakNet::BitStream* bitInput)
{
	Vector3D position;
	int ID;
	bitInput->Read(ID);
	bitInput->Read(position);

	ENGINE_STACK::PLAYER_SHOOT* pw = new ENGINE_STACK::PLAYER_SHOOT(ID, position);
	g_CCore->GetEngineStack()->AddMessage(ES_THROWGRANADE, (DWORD)pw);
}

void CGameSync::onPlayerDie(RakNet::BitStream* bitInput)
{
	int ID, reason, part;
	bitInput->Read(ID);
	bitInput->Read(reason);
	bitInput->Read(part);
	
	char buff[255];
	sprintf(buff, "[NM] PlayerDeath %i", ID);
	g_CCore->GetLog()->AddLog(buff);

	ENGINE_STACK::KILL_PED_EX* pw = new ENGINE_STACK::KILL_PED_EX(ID, reason, part);
	g_CCore->GetEngineStack()->AddMessage(ES_PLAYERDEATHEX, (DWORD)pw);
}

void CGameSync::onPlayerDieEnd(RakNet::BitStream* bitInput)
{
		int ID;// reason, part;
		bitInput->Read(ID);

		char buff[500];
		sprintf(buff, "[NM] PlayerDeathEND %i", ID);
		g_CCore->GetLog()->AddLog(buff);

		g_CCore->GetEngineStack()->AddMessage(ES_PLAYERDEATH_END, ID);
}


void CGameSync::onPlayerIsPutToCar(RakNet::BitStream* bitInput)
{
	int ID, carID, seatID;
	bitInput->Read(ID);
	bitInput->Read(carID);
	bitInput->Read(seatID);
	
	char buff[255];
	sprintf(buff, "[Nm] PUT TO VEH %d %d %d", ID, carID, seatID);
	g_CCore->GetLog()->AddLog(buff);
	
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(carID);
	if (veh)
	{
		if (ID == g_CCore->GetLocalPlayer()->GetOurID())
		{
			g_CCore->GetLocalPlayer()->IDinCar = carID;
			g_CCore->GetLocalPlayer()->SetIsOnFoot(false);
			veh->PlayerEnter(ID, seatID);
		}
		else
		{
			CPed* ped = g_CCore->GetPedPool()->Return(ID);
			if (ped)
			{
				ped->InCar = carID;
				ped->SetIsOnFoot(false);
			}
			veh->PlayerEnter(ID, seatID);
		}
		ENGINE_STACK::PLAYER_ENTER_VEH* data = new ENGINE_STACK::PLAYER_ENTER_VEH(ID, carID, seatID);
		g_CCore->GetEngineStack()->AddMessage(ES_PLAYER_PUT_TO_VEH, (DWORD)data);
	}

}
void CGameSync::onPlayerIsKickedFromCar(RakNet::BitStream* bitInput)
{
	int ID, carID;
	bitInput->Read(ID);
	bitInput->Read(carID);
	
	char buff[255];
	sprintf(buff, "[Nm] KICK OUT VEH %d %d", ID, carID);
	g_CCore->GetLog()->AddLog(buff);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(carID);
	if (veh)
	{
		// isLocalPlayer
		if (ID == g_CCore->GetLocalPlayer()->GetOurID())
		{
			//TODO InCar
			g_CCore->GetLocalPlayer()->IDinCar = -1;
			g_CCore->GetLocalPlayer()->SetIsOnFoot(true);
			veh->PlayerExit(ID);
		}
		else
		{
			CPed* ped = g_CCore->GetPedPool()->Return(ID);
			if (ped)
			{
				//TODO InCar
				ped->InCar = -1;
				ped->SetIsOnFoot(true);
			}
			veh->PlayerExit(ID);
		}
		ENGINE_STACK::PLAYER_EXIT_VEH* data = new ENGINE_STACK::PLAYER_EXIT_VEH(ID, carID);
		g_CCore->GetEngineStack()->AddMessage(ES_PLAYER_KICK_OUT_VEHICLE, (DWORD)data);
	}
}

void CGameSync::onGUIMoneyChange(RakNet::BitStream* bitInput)
{
		int money;
		bitInput->Read(money);

		char buff[255];
		sprintf(buff, "[Nm] SET MONEY %d", money);
		g_CCore->GetLog()->AddLog(buff);
		g_CCore->GetLocalPlayer()->SetMoney(money);
}

void CGameSync::onGUIEnableChange(RakNet::BitStream* bitInput)
{
	int enable;
	bitInput->Read(enable);

	char buff[255];
	sprintf(buff, "[Nm] ENABLE MONEY %d", enable);
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetLocalPlayer()->EnableMoney(enable);
	
}

void CGameSync::onGUIEnableNametags(RakNet::BitStream* bitInput)
{
	bool status;
	bitInput->Read(status);
	
	char buff[255];
	sprintf(buff, "[Nm] SET NAMETAG %d", status);
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetGraphics()->SetShowNameTags(status);
}

void CGameSync::onPlayerPositionChange(RakNet::BitStream* bitInput)
{
	int ID;
	Vector3D pos;
	bitInput->Read(ID);
	bitInput->Read(pos);
	char buff[255];
	sprintf(buff, "[Nm] SET POSITION %d", ID);
	g_CCore->GetLog()->AddLog(buff);

	if (ID == g_CCore->GetLocalPlayer()->GetOurID())
	{
		g_CCore->GetLocalPlayer()->SetLocalPos(pos);
		g_CCore->GetGame()->SetPlayerPosition(g_CCore->GetLocalPlayer()->GetEntity(), pos);
	}
	else
	{
		CPed* ped = g_CCore->GetPedPool()->Return(ID);
		if (ped != NULL)
		{
			ped->SetPosition(pos);
			g_CCore->GetGame()->SetPlayerPosition(ped->GetEntity(), pos);
		}

	}
}

void CGameSync::onPlayerRotationChange(RakNet::BitStream* bitInput)
{
	int ID;
	Vector3D rot;
	bitInput->Read(ID);
	bitInput->Read(rot);
	char buff[255];
	sprintf(buff, "[Nm] SET ROTATION %d", ID);
	g_CCore->GetLog()->AddLog(buff);

	if (ID == g_CCore->GetLocalPlayer()->GetOurID())
	{
		g_CCore->GetLocalPlayer()->SetLocalRot(rot);
	}
	else
	{
		CPed* ped = g_CCore->GetPedPool()->Return(ID);
		ped->SetRotation(rot);
	}
}

void CGameSync::onCameraIsSet(RakNet::BitStream* bitInput)
{
	Vector3D pos, rot;
	bitInput->Read(pos);
	bitInput->Read(rot);
	ENGINE_STACK::CAMERA_SET* data = new ENGINE_STACK::CAMERA_SET(pos, rot);
	g_CCore->GetEngineStack()->AddMessage(ES_CAMERASETPOS, (DWORD)data);
}


void CGameSync::onCameraReturn(RakNet::BitStream* bitInput)
{
	g_CCore->GetEngineStack()->AddMessage(ES_CAMERAUNLOCK, NULL);
}


void CGameSync::onCameraSetSwing(RakNet::BitStream* bitInput)
{
	int state;
	float force;

	bitInput->Read(state);
	bitInput->Read(force);

	char buff[255];
	sprintf(buff, "[Nm] PLAYER SET CAMERA SWING %d %f", state, force);
	g_CCore->GetLog()->AddLog(buff);

	g_CCore->GetGame()->CameraSetSwing((byte)state, force);
}


void CGameSync::onCameraSetFOV(RakNet::BitStream* bitInput)
{
	float FOV;
	bitInput->Read(FOV);
	char buff[255];
	sprintf(buff, "[Nm] PLAYER SET CAMERA FOV %f", FOV);
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetGame()->CameraSetFov(FOV);
}

void CGameSync::onMusicIsTurnedOff(RakNet::BitStream* bitInput)
{
	int state;
	bitInput->Read(state);

	char buff[255];
	sprintf(buff, "[Nm] TOGGLE CITY MUSIC %d", state);
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetGame()->ToggleCityMusic(state);
}


void CGameSync::onWeatherIsChanged(RakNet::BitStream* bitInput)
{
	int  param_val;
	char parameter[300];
	bitInput->Read(parameter);
	bitInput->Read(param_val);

	char buff[255];
	sprintf(buff, "[Nm] PLAYER SET WEATHER PARAM %s, %d", parameter, param_val);
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetGame()->WeatherSetParam(parameter, param_val);
}

void CGameSync::onObjectivesAreSet(RakNet::BitStream* bitInput)
{
	char text[900];
	bitInput->Read(text);
	char buff[255];
	sprintf(buff, "[Nm] PLAYER SET  OBJECTIVE %s", text);
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetGame()->SetObjective(text);
}

void CGameSync::onObjectivesAreCleared(RakNet::BitStream* bitInput)
{
	int ID;
	bitInput->Read(ID);
	char buff[255];
	sprintf(buff, "[Nm] PLAYER CLEAR OBJECTIVE");
	g_CCore->GetLog()->AddLog(buff);
	g_CCore->GetGame()->ClearObjective();
}

void  CGameSync::onConsoleTextIsAdded(RakNet::BitStream* bitInput)
{
	char color[50];
	char text[900];

	bitInput->Read(color);
	bitInput->Read(text);

	DWORD color_ex = Tools::GetARGBFromString(color);

	char buff[255];
	sprintf(buff, "[Nm] PLAYER ADD CONSOLE TEXT %X, %s", color_ex, text);
	g_CCore->GetLog()->AddLog(buff);

	g_CCore->GetGame()->ConsoleAddText(text, color_ex);
}


void CGameSync::onPlayerControlsAreLocked(RakNet::BitStream* bitInput)
{
	unsigned int status;
	bitInput->Read(status);
	g_CCore->GetGame()->SetLockControls(status == 1);
}


void  CGameSync::onVehicleIsSpawned(RakNet::BitStream* bitInput, RakNet::TimeMS timestamp)
{
	VEH::CREATE vehicle;
	bitInput->Read(vehicle);

	// log it
	char buff[255];
	sprintf(buff, "LHMP_VEHICLE_CREATE %d %d %d %d %d %d", vehicle.ID, vehicle.seat[0], vehicle.seat[1], vehicle.seat[2], vehicle.seat[3], vehicle.siren);
	g_CCore->GetLog()->AddLog(buff);

	// create a new instance in vehicle pool
	g_CCore->GetVehiclePool()->New(vehicle.ID, vehicle.skinID, vehicle.position, vehicle.rotation, vehicle.isSpawned);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehicle.ID);
	if (veh)
	{
		veh->SetActive(vehicle.isSpawned);
		veh->SetPosition(vehicle.position);
		veh->SetRotation(vehicle.rotation);
		veh->SetTimeStamp(timestamp);
		veh->SetUpInterpolation();
		veh->SetDamage(vehicle.damage);
		veh->ToggleRoof(vehicle.roofState);
		veh->SetSirenState(vehicle.siren);
		for (int i = 0; i < 4; i++)
			veh->SetSeat(i, vehicle.seat[i]);
		g_CCore->GetLog()->AddLog("LHMP_VEHICLE_CREATE");
	}
	else {
		// TODO: handle error
	}
}


void CGameSync::onVehicleIsDeleted(RakNet::BitStream* bitInput)
{
	int Id;
	bitInput->Read(Id);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(Id);
	if (veh != NULL)
	{
		ENGINE_STACK::VEH_DELETEVEH* data = new ENGINE_STACK::VEH_DELETEVEH(Id, veh->GetEntity());
		g_CCore->GetEngineStack()->AddMessage(ES_DELETECAR, (DWORD)data);
		g_CCore->GetVehiclePool()->Delete(Id);
	}

}

void CGameSync::onVehicleEngineStateChange(RakNet::BitStream* bitInput)
{
	int ID;
	bool state;

	bitInput->Read(ID);
	bitInput->Read(state);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);

	if (veh && g_CCore->GetLocalPlayer()->GetOurID() != veh->GetSeat(0))
	{
		veh->ToggleEngine(state);
		char buff[255];
		sprintf(buff, "[Nm] TOGGLE ENGINE %d STATE: %d", ID, state);
		g_CCore->GetLog()->AddLog(buff);
	}
}


void CGameSync::onVehicleRoofIsChanged(RakNet::BitStream* bitInput)
{
	int ID, state;
	bitInput->Read(ID);
	bitInput->Read(state);

	char buff[255];
	sprintf(buff, "[Nm] TOGGLE ROOF %d STATE: %d", ID, state);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->ToggleRoof((byte)state);
	}
}


void CGameSync::onVehicleSirenIsSet(RakNet::BitStream* bitInput)
{
	int ID;
	int state;
	bitInput->Read(ID);
	bitInput->Read(state);

	char buff[255];
	sprintf(buff, "[Nm] TOGGLE SIREN %d STATE: %d", ID, state);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->SetSirenState((state == 1));
	}
}

void CGameSync::onVehicleFuelIsSet(RakNet::BitStream* bitInput)
{
	int ID;
	float fuel;
	bitInput->Read(ID);
	bitInput->Read(fuel);

	char buff[255];
	sprintf(buff, "[Nm] SET FUEL %f", fuel);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->SetFuel(fuel);
	}
}

void CGameSync::onVehicleIsEntered(RakNet::BitStream* bitInput)
{
	int Id, vehId, seatId;
	bitInput->Read(Id);
	bitInput->Read(vehId);
	bitInput->Read(seatId);
	char buff[255];
	sprintf(buff, "ENTER VEHICLE %d %d %d", Id, vehId, seatId);
	g_CCore->GetLog()->AddLog(buff);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
	if (veh != NULL)
	{
		CPed* ped = g_CCore->GetPedPool()->Return(Id);
		if (ped != NULL)
		{
			ped->InCar = vehId;
			veh->PlayerEnter(Id, seatId);
			ENGINE_STACK::PLAYER_ENTER_VEH* data = new ENGINE_STACK::PLAYER_ENTER_VEH(Id, vehId, seatId);
			g_CCore->GetEngineStack()->AddMessage(ES_PLAYER_ENTER_VEH, (DWORD)data);
			ped->SetIsOnFoot(false);
		}
	}
}

// when player exits a car
void CGameSync::onVehicleIsExited(RakNet::BitStream* bitInput)
{
	int Id, vehId;
	bitInput->Read(Id);
	bitInput->Read(vehId);
	char buff[255];
	sprintf(buff, "EXIT VEHICLE %d %d", Id, vehId);
	g_CCore->GetLog()->AddLog(buff);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
	CPed* ped = g_CCore->GetPedPool()->Return(Id);
	if (veh && ped)
	{
		ped->InCar = -1;
		veh->PlayerExit(Id);
		ENGINE_STACK::PLAYER_ENTER_VEH* data = new ENGINE_STACK::PLAYER_ENTER_VEH(Id, vehId, NULL);
		g_CCore->GetEngineStack()->AddMessage(ES_PLAYER_EXIT_VEH, (DWORD)data);
		ped->SetIsOnFoot(true);
	}
}

void CGameSync::onVehicleSyncArrives(RakNet::BitStream* bitInput, RakNet::TimeMS timestamp)
{
	VEH::SYNC syncData;
	bitInput->Read(syncData);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(syncData.ID);
	if (veh != NULL)
	{
		if (timestamp - veh->GetTimeStamp() > 0)
		{

			//veh->SetVehiclePosition(syncData.position);
			veh->SetRotation(syncData.rotation);
			veh->SetWheelsAngle(syncData.wheels);
			veh->SetSpeed(syncData.speed);
			veh->SetHornState(syncData.horn);
			veh->SetTimeStamp(timestamp);
			veh->SetSecondRot(syncData.secondRot);
			veh->SetPosition(syncData.position);
			veh->SetIsOnGas(syncData.gasOn);
			veh->SetUpInterpolation();
		}
	}
}

void CGameSync::onVehicleIsJacked(RakNet::BitStream* bitInput)
{
	int Id, vehId, seatId;
	bitInput->Read(Id);
	bitInput->Read(vehId);
	bitInput->Read(seatId);

	char buff[255];
	sprintf(buff, "CAR JACK %d %d %d", Id, vehId, seatId);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
	if (veh)
	{
		int jackedID = veh->GetSeat(seatId);
		veh->PlayerExit(jackedID);
		if (jackedID != g_CCore->GetLocalPlayer()->GetOurID())
		{
			CPed* pedJacked = g_CCore->GetPedPool()->Return(jackedID);
			if (pedJacked != NULL)
			{
				pedJacked->InCar = -1;
				pedJacked->SetIsOnFoot(true);
			}
		}
		else
		{
			g_CCore->GetLocalPlayer()->IDinCar = -1;
			g_CCore->GetLocalPlayer()->SetIsOnFoot(true);

		}
		veh->PlayerExit(veh->GetSeat(seatId));
		ENGINE_STACK::VEH_JACK* data = new ENGINE_STACK::VEH_JACK(Id, vehId, seatId);
		g_CCore->GetEngineStack()->AddMessage(ES_VEHICLE_JACK, (DWORD)data);
	}
}


void CGameSync::onVehicleIsDamaged(RakNet::BitStream* bitInput)
{
	int vehId;
	float damage;
	bitInput->Read(vehId);
	bitInput->Read(damage);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
	if (veh != NULL)
	{
		veh->SetDamage(damage);
	}
}


void CGameSync::onVehicleIsDamagedByShot(RakNet::BitStream* bitInput)
{
	int vehId;
	byte damage;
	bitInput->Read(vehId);
	bitInput->Read(damage);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(vehId);
	if (veh != NULL)
	{
		veh->SetShotDamage(damage);
	}
}


void CGameSync::onVehiclePositionIsSet(RakNet::BitStream* bitInput)
{
	int ID;
	Vector3D pos;
	bitInput->Read(ID);
	bitInput->Read(pos);
	char buff[255];
	sprintf(buff, "[Nm] SET POSITION %d", ID);
	g_CCore->GetLog()->AddLog(buff);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		if (veh->GetSeat(0) == g_CCore->GetLocalPlayer()->GetOurID())
		{
			// TODO: struct
			veh->SetPosition(pos);
			*(float*)(veh->GetEntity() + 0x40C) = pos.x;
			*(float*)(veh->GetEntity() + 0x410) = pos.y;
			*(float*)(veh->GetEntity() + 0x414) = pos.z;
		}
	}

}

void CGameSync::onVehicleRotationIsSet(RakNet::BitStream* bitInput)
{
	int ID;
	Vector3D rot;
	bitInput->Read(ID);
	bitInput->Read(rot);
	char buff[255];
	sprintf(buff, "[Nm] SET ROTATION %d", ID);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		if (veh->GetSeat(0) == g_CCore->GetLocalPlayer()->GetOurID())
		{
			// TODO: struct
			veh->SetRotation(rot);
			*(float*)(veh->GetEntity() + 0xD28) = rot.x;
			*(float*)(veh->GetEntity() + 0xD2C) = rot.y;
			*(float*)(veh->GetEntity() + 0xD30) = rot.z;
		}
	}
}

void CGameSync::onVehicleSpeedIsSet(RakNet::BitStream* bitInput)
{
	int ID;
	Vector3D speed;
	bitInput->Read(ID);
	bitInput->Read(speed);
	char buff[255];
	sprintf(buff, "[Nm] SET SPEED %d", ID);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		if (veh->GetSeat(0) == g_CCore->GetLocalPlayer()->GetOurID())
		{
			veh->SetSpeed(speed);
		}
	}
}


void CGameSync::onVehicleRespawned(RakNet::BitStream* bitInput)
{
	int ID;
	bitInput->Read(ID);
	Vector3D pos, rot;
	bitInput->Read(pos);
	bitInput->Read(rot);


	char buff[255];
	sprintf(buff, "[Nm] On Car Respawn %d", ID);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->SetPosition(pos);
		veh->SetRotation(rot);

		veh->SetDamage(100.0f);
		veh->SetShotDamage(10);
		veh->SetActive(true);
		g_CCore->GetEngineStack()->AddMessage(ES_CARRESPAWN, ID);
	}
}


void CGameSync::onVehicleExploded(RakNet::BitStream* bitInput)
{
	int ID;
	bitInput->Read(ID);
	char buff[255];
	sprintf(buff, "[Nm] On Exploded %d", ID);
	g_CCore->GetLog()->AddLog(buff);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		g_CCore->GetEngineStack()->AddMessage(ES_CAREXPLODE, ID);
		veh->SetActive(false);
	}
}


void CGameSync::onDoorStateChange(RakNet::BitStream* bitInput)
{
	char name[200];
	bool state, facing;
	bitInput->Read(name);
	bitInput->Read(state);
	bitInput->Read(facing);

	char buff[250];
	sprintf(buff, "[Nm] SET DOOR '%s'[%d] STATE %d %d", name, strlen(name), state, facing);
	g_CCore->GetLog()->AddLog(buff);
	if (strlen(name) > 0)
	{

		ENGINE_STACK::DOOR_SET_STATE* data = new ENGINE_STACK::DOOR_SET_STATE(name, state, facing);
		g_CCore->GetEngineStack()->AddMessage(ES_DOOR_SET_STATE, (DWORD)data);
	}
}


void CGameSync::onMapChange(RakNet::BitStream* bitInput)
{
	char name[200];
	bitInput->Read(name);

	sprintf(g_CCore->GetGame()->mapName, name);
	char buff[250];
	sprintf(buff, "[Nm] SET map %s", name);
	g_CCore->GetLog()->AddLog(buff);
	
}

void CGameSync::onPickupIsCreated(RakNet::BitStream* bitInput)
{
	g_CCore->GetLog()->AddLog("[NM] Pickup create");
	int ID;
	char model[250];
	bool isVisible;
	Vector3D position;
	float size;
	bitInput->Read(ID);
	bitInput->Read(model);
	bitInput->Read(position);
	bitInput->Read(size);
	bitInput->Read(isVisible);
	g_CCore->GetPickupPool()->New(ID, model, position, size, isVisible);
	g_CCore->GetEngineStack()->AddMessage(ES_CREATEPICKUP, (DWORD)ID);
}

void CGameSync::onPickupIsDeleted(RakNet::BitStream* bitInput)
{
	unsigned short ID;
	bitInput->Read(ID);

	// if ID is not invalid
	if (ID < MAX_PICKUPS)
	{
		CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
		if (pickup)
		{
			g_CCore->GetEngineStack()->AddMessage(ES_DELETEPICKUP, (DWORD)ID);
		}
	}
}

void CGameSync::onPickupIsHidden(RakNet::BitStream* bitInput)
{
	int ID;
	bool shouldBeVisible;
	bitInput->Read(ID);
	bitInput->Read(shouldBeVisible);
	// if ID is not invalid
	if (ID < MAX_PICKUPS)
	{
		CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
		if (pickup)
		{
			pickup->SetVisible(shouldBeVisible);
			g_CCore->GetEngineStack()->AddMessage(ES_SETPICKUPVISIBLE, (DWORD)ID);

		}
	}
}
void CGameSync::HandlePacket(RakNet::Packet* packet, RakNet::TimeMS timestamp)
{
	int offset = 1;
	if (timestamp != NULL)
	{
		//offset += sizeof(RakNet::MessageID);
		offset += sizeof(RakNet::MessageID) + sizeof(RakNet::TimeMS);
	}
	RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
	switch (packet->data[offset])
	{
	case LHMP_PLAYER_CHAT_MESSAGE:
		this->onTextMessage(&bsIn);
		break;
	case LHMP_PLAYER_SENT_SYNC_ON_FOOT:
		this->onPlayerUpdateOnFoot(&bsIn, timestamp);
		break;
	case LHMP_PLAYER_SENT_CAR_UPDATE:
		this->onPlayerUpdateInCar(&bsIn);
		break;
	case LHMP_PLAYER_CONNECT:
		this->onPlayerConnect(&bsIn);
		break;
	case LHMP_PLAYER_CHANGESKIN:
		this->onPlayerChangeSkin(&bsIn);
		break;
	case LHMP_PLAYER_DISCONNECT:
		this->onPlayerDisconnect(&bsIn);
	break;

	// ASAP, it's obsolete & unsupported
	// TODO: it has been replaced with PLAYANIM_STRING (for custom anims)
	case LHMP_PLAYER_PLAYANIM:
		break;
	case LHMP_PLAYER_PLAYANIM_STRING:
		this->onPlayerPlayAnimation(&bsIn);
		break;
	case LHMP_PLAYER_PLAYSOUND_STRING:
		this->playSound(&bsIn);
		break;
	case LHMP_PLAYER_RESPAWN:
		this->onPlayerRespawn(&bsIn);
		break;
	case LHMP_PLAYER_PING:
		this->onPlayerPingArrives(&bsIn);
		break;
	case LHMP_PLAYER_ADDWEAPON:
		this->onPlayerAddWeapon(&bsIn);
		break;
	case LHMP_PLAYER_DELETEWEAPON:
		this->onPlayerDeleteWeapon(&bsIn);
	case LHMP_PLAYER_SWITCHWEAPON:
		this->onPlayerDeleteWeapon(&bsIn);
		break;
	case LHMP_PLAYER_SHOOT:
		this->onPlayerShoot(&bsIn);
		break;
	case LHMP_PLAYER_THROWGRANADE:
		this->onPlayerThrowGranade(&bsIn);
		break;
	case LHMP_PLAYER_DEATH:
		this->onPlayerDie(&bsIn);
		break;
	case LHMP_PLAYER_DEATH_END:
		this->onPlayerDieEnd(&bsIn);
		break;
	case LHMP_PLAYER_PUT_TO_VEHICLE:
		this->onPlayerIsPutToCar(&bsIn);
		break;
	case LHMP_PLAYER_KICK_OUT_VEHICLE:
		this->onPlayerIsKickedFromCar(&bsIn);
		break;
	case LHMP_PLAYER_SET_HEALTH:
		this->onPlayerIsKickedFromCar(&bsIn);
		break;
	case LHMP_PLAYER_SET_MONEY:
		this->onGUIMoneyChange(&bsIn);
		break;
	case LHMP_PLAYER_ENABLE_MONEY:
		this->onGUIEnableChange(&bsIn);
		break;
	case LHMP_PLAYER_SET_NAMETAG:
		this->onGUIEnableNametags(&bsIn);
		break;
	case LHMP_PLAYER_SET_POSITION:
		this->onPlayerPositionChange(&bsIn);
		break;
	case LHMP_PLAYER_SET_ROTATION:
		this->onPlayerRotationChange(&bsIn);
	break;
	case LHMP_PLAYER_SET_CAMERA:
		this->onCameraIsSet(&bsIn);
	break;
	case LHMP_PLAYER_SET_CAMERA_DEFAULT:
		this->onCameraReturn(&bsIn);
	break;
	case LHMP_PLAYER_SET_CAMERA_SWING:
		this->onCameraSetSwing(&bsIn);
	break;
	case LHMP_PLAYER_SET_CAMERA_FOV:
		this->onCameraSetFOV(&bsIn);
	break;
	case LHMP_PLAYER_TOGGLE_CITY_MUSIC:
		this->onMusicIsTurnedOff(&bsIn);
	break;
	case LHMP_PLAYER_SET_WEATHER_PARAM:
		this->onWeatherIsChanged(&bsIn);
	break;
	case LHMP_PLAYER_SET_OBJECTIVE:
		this->onObjectivesAreSet(&bsIn);
	break;
	case LHMP_PLAYER_CLEAR_OBJECTIVE:
		this->onObjectivesAreCleared(&bsIn);
	break;
	case LHMP_PLAYER_ADD_CONSOLE_TEXT:
		this->onConsoleTextIsAdded(&bsIn);
	break;
	case LHMP_PLAYER_LOCK_CONTROLS:
		this->onPlayerControlsAreLocked(&bsIn);
	break;
	/*STOP here*/
	case LHMP_VEHICLE_CREATE:
		this->onVehicleIsSpawned(&bsIn,timestamp);
	break;
	case LHMP_VEHICLE_TOGGLE_ENGINE:
		this->onVehicleEngineStateChange(&bsIn);
	break;
	case LHMP_VEHICLE_TOGGLE_ROOF:
		this->onVehicleRoofIsChanged(&bsIn);
	break;
	case LHMP_VEHICLE_TOGGLE_SIREN:
		this->onVehicleSirenIsSet(&bsIn);
	break;

	case LHMP_VEHICLE_SET_FUEL:
		this->onVehicleFuelIsSet(&bsIn);
	break;
	case LHMP_PLAYER_ENTERED_VEHICLE:
		this->onVehicleIsEntered(&bsIn);
	break;
	case LHMP_PLAYER_EXIT_VEHICLE:
		this->onVehicleIsExited(&bsIn);
	break;
	case LHMP_PLAYER_EXIT_VEHICLE_FINISH:
		this->onVehicleIsEntered(&bsIn);
	break;
	case LHMP_VEHICLE_SYNC:
		this->onVehicleSyncArrives(&bsIn, timestamp);
	break;
	case LHMP_VEHICLE_JACK:
		this->onVehicleIsJacked(&bsIn);
	break;
	case LHMP_VEHICLE_DELETE:
		this->onVehicleIsDeleted(&bsIn);
	break;
	case LHMP_VEHICLE_DAMAGE:
		this->onVehicleIsDamaged(&bsIn);
	break;
	case LHMP_VEHICLE_SHOTDAMAGE:
		this->onVehicleIsDamagedByShot(&bsIn);
	break;
	case LHMP_VEHICLE_SET_POSITION:
		this->onVehiclePositionIsSet(&bsIn);
	break;
	case LHMP_VEHICLE_SET_ROTATION:
		this->onVehicleRotationIsSet(&bsIn);
	break;
	case LHMP_VEHICLE_SET_SPEED:
		this->onVehicleSpeedIsSet(&bsIn);
	break;
	case LHMP_VEHICLE_ON_EXPLODED:
		this->onVehicleExploded(&bsIn);
	break;
	case LHMP_VEHICLE_RESPAWN:
		this->onVehicleRespawned(&bsIn);
	break;
	case LHMP_DOOR_SET_STATE:
		this->onDoorStateChange(&bsIn);
	break;
	case LHMP_SCRIPT_CHANGE_MAP:
		this->onMapChange(&bsIn);
	break;

	/*ok, not sure*/
	//TODO - move to CNetwork
	case LHMP_FILE_SEND:
	{
		RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
		FileList* listFile = new FileList();
		listFile->Deserialize(&bsIn);
		g_CCore->GetChat()->AddMessage("FileList arrived");
		char buff[200];
		sprintf(buff, "List size: %u", listFile->fileList.Size());
		g_CCore->GetChat()->AddMessage(buff);
		listFile->WriteDataToDisk("lhmp/files/");

	}
	break;

	case LHMP_SCRIPT_CALLFUNC:
	{
		RakNet::BitStream bsIn(packet->data + offset + 1, packet->length - offset - 1, false);
		char script_name[500];
		char func_name[500];

		bsIn.Read(script_name);
		bsIn.Read(func_name);
		g_CCore->GetSquirrel()->callClientFunc(script_name, func_name, &bsIn);
	}
	break;


	case LHMP_PICKUP_CREATE:
		this->onPickupIsCreated(&bsIn);
	break;
	case LHMP_PICKUP_DELETE:
		this->onPickupIsDeleted(&bsIn);
	break;
	case LHMP_PICKUP_SETVISIBLE:
		this->onPickupIsHidden(&bsIn);
	break;
	}
}