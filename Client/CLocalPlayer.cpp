#include "CLocalPlayer.h"
//#include "structures.h"
#include "../shared/structures.h"
#include "CNetwork.h"
#include <fstream>
#include "CCore.h"
#include "GetTime.h"

extern CCore *g_CCore;
CLocalPlayer::CLocalPlayer()
{
	IDinCar			= -1;
	bIsOnFoot		= true;
	ping			= -1;
	this->ourSkin	= 0;
}

CLocalPlayer::~CLocalPlayer()
{
}

void CLocalPlayer::Init()
{
	this->ourAnim = -1;
}

void CLocalPlayer::SetMoney(int money)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetMoney", LOG_ALL);
	__asm
	{
		MOV ESI, money
			PUSH ESI
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x00425390
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005F5C30
			CALL EAX
	}
}

void CLocalPlayer::EnableMoney(int enable)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::EnableMoney", LOG_ALL);
	BYTE benable = (BYTE)enable;


	__asm
	{
		MOV ECX, DWORD PTR DS : [0x65115C]
			MOV DL, benable
			PUSH EDX
			MOV EAX, 0x00425390
			CALL EAX
			MOV ECX, EAX
			MOV EAX, 0x005F5BF0
			CALL EAX
	}
}

void CLocalPlayer::SetLocalPos(Vector3D vector)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetLocalPos", LOG_ALL);
	if (this->GetEntity() != NULL)
	{
		PED* ped = g_CCore->GetGame()->GetLocalPED();
		ped->object.position = vector;
		/**(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x24) = vector.x;
		*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x28) = vector.y;
		*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x2C) = vector.z;*/
	}
}


float CLocalPlayer::GetCarAim()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetCarAim", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->inCarRotation;
	//return *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x5F4);
}
float CLocalPlayer::GetHealth()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetHealth", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->health;
	//return *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x644);
}
byte CLocalPlayer::GetStatus()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetStatus", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->animStateLocal;
	//return	*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x70);
}
bool CLocalPlayer::GetIsAiming()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetIsAiming", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return (ped->isAiming == 1);
	//return	(*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x1E5) == 1);
}
bool CLocalPlayer::GetIsDucking()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetIsDucking", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return (ped->isDucking == 1);
	//return	(*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x1E4) == 1);
}

bool CLocalPlayer::GetIsCarAnim()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetLocalPED", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return (ped->isInAnimWithCarLocal == 1);
	//return	(*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x71) == 1);
}
bool CLocalPlayer::IsOnFoot()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::IsOnFoot", LOG_ALL);
		return bIsOnFoot;
}
void CLocalPlayer::SetIsOnFoot(bool b)
{
	this->bIsOnFoot = b;
	if (b)
		g_CCore->GetLog()->AddLog("CLocalPlayer::SetIsOnFoot 1", LOG_ALL);
	else
		g_CCore->GetLog()->AddLog("CLocalPlayer::SetIsOnFoot 0", LOG_ALL);
}

void CLocalPlayer::Pulse()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::Pulse", LOG_ALL);
	if (this->GetEntity() != NULL)
	{
		if (this->IsOnFoot())
		{
			// Send on-foot sync
			SYNC::ON_FOOT_SYNC syncData;
			syncData.position = this->GetLocalPos();
			Vector3D rot = this->GetLocalRot();
			syncData.degree = rot.x;
			syncData.degree_second = rot.z;
			syncData.degree_third = rot.x;
			syncData.status = this->GetStatus();
			syncData.health = this->GetHealth();
			syncData.isDucking = this->GetIsDucking();
			syncData.isAim = this->GetIsAiming();
			syncData.isCarAnim = this->GetIsCarAnim();


			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_TIMESTAMP);
			bsOut.Write(RakNet::GetTimeMS());
			bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SENT_SYNC_ON_FOOT);
			bsOut.Write(syncData);
			g_CCore->GetNetwork()->SendServerMessage(&bsOut, LOW_PRIORITY, UNRELIABLE);

		}
		else
		{
			// Send in-car sync
			if (this->IDinCar != -1)
			{
				CVehicle* veh = g_CCore->GetVehiclePool()->Return(IDinCar);
				if (veh != NULL)
					veh->SendSync();
			}

			SYNC::IN_CAR syncData;
			syncData.ID = this->GetOurID();
			syncData.health = this->GetHealth();
			syncData.isAiming = this->GetIsAiming();
			syncData.aim = this->GetCarAim();
			RakNet::BitStream bsOut;
			bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SENT_CAR_UPDATE);
			bsOut.Write(syncData);
			g_CCore->GetNetwork()->SendServerMessage(&bsOut, LOW_PRIORITY, UNRELIABLE);

		}
	}
}

void CLocalPlayer::SetID(int id)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetID", LOG_ALL);
	this->ourID = id;
}

int CLocalPlayer::GetOurID()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetOurID", LOG_ALL);
	return this->ourID;
}

Vector3D CLocalPlayer::GetLocalPos()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetLocalPos", LOG_ALL);
	Vector3D pos;
	if (this->IDinCar != -1)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(IDinCar);
		if (veh != NULL)
		{
			return veh->GetPosition();
		}
		else {
			pos.x = 0.0f;
			pos.y = 0.0f;
			pos.z = 0.0f;
			return pos;
		}
	}
	else
	{
		if (this->GetEntity() != NULL)
		{
			PED* ped = g_CCore->GetGame()->GetLocalPED();
			return ped->object.position;
			/*pos.x = *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x24);
			pos.y = *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x28);
			pos.z = *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x2C);
			return pos;*/
		}
		else {
			pos.x = 0.0f;
			pos.y = 0.0f;
			pos.z = 0.0f;
			return pos;
		}
	}
	/*}
	else
	{
		Vector3D pos;
		pos.x = *(float*) (car + 0x40C);
		pos.y = *(float*)(car + 0x410);
		pos.z = *(float*)(car + 0x414);
		return pos;
	}*/
}

char*	CLocalPlayer::GetNickname()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetNickname", LOG_ALL);
	return nickname;
}
void	CLocalPlayer::SetNickname(char* nick)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetNickname", LOG_ALL);
	sprintf(nickname,"%s",nick);
}

void	CLocalPlayer::SetPing(int p)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetPing", LOG_ALL);
	ping = p;
}

int		CLocalPlayer::GetPing()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetPing", LOG_ALL);
	return ping;
}

DWORD CLocalPlayer::GetBase()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetBase", LOG_ALL);
	if ((*(DWORD*)0x006F9464) != NULL)
	{
		return *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
	}
	else {
		return NULL;
	}
}


DWORD CLocalPlayer::GetEntity()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetEntity", LOG_ALL);
	if ((*(DWORD*)0x006F9464) != NULL)
	{
		return *(DWORD*)(*(DWORD*)(0x006F9464) + 0xE4);
	}
	else {
		return NULL;
	}
}

Vector3D	CLocalPlayer::GetLocalRot()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::GetLocalRot", LOG_ALL);
	if (this->IDinCar != -1)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(IDinCar);
		if (veh != NULL)
		{
			return veh->GetRotation();
		} else {
			Vector3D pos;
			pos.x = 0.0f;
			pos.y = 0.0f;
			pos.z = 0.0f;
			return pos;
		}
	}
	else
	{
		Vector3D rot;
		PED* ped = g_CCore->GetGame()->GetLocalPED();
		/*rot.x = *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x30);
		rot.y = *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x34);
		rot.z = *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x38);*/
		rot = ped->object.rotation;
		return rot;
	}
}


void	CLocalPlayer::SetLocalRot(Vector3D rot)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetLocalRot", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	ped->object.rotation = rot;
	/**(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x30) = rot.x;
	*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x34) = rot.y;
	*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x38) = rot.z;*/
}

void	CLocalPlayer::SetHealth(float f)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetHealth", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	ped->health = f;
	//*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x644) = f;

	// ingame GUI change
	(*(DWORD*)0x00661538)	= (unsigned int)(f/2);
}


bool	CLocalPlayer::HasWeapon(int wepID)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::HasWeapon", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	for (int i = 0; i < 7; i++)
	{
		//if (*(DWORD*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + (0x4A0 + i * 0x10)) == wepID)
		if (ped->inventary.slot[i].weaponType == wepID)
			return true;
	}
	return false;
}
void	CLocalPlayer::SetWeapon(int wepID, int ammo, int ammo2)
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::SetWeapon", LOG_ALL);
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	for (int i = 0; i < 7; i++)
	{
		//if (*(DWORD*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + (0x4A0 + i * 0x10)) == wepID)
		if (ped->inventary.slot[i].weaponType == wepID)
		{
			//*(DWORD*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + (0x4A0 + i * 0x10) + 0x4) += ammo;
			//*(DWORD*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + (0x4A0 + i * 0x10) + 0x8) += ammo2;
			ped->inventary.slot[i].ammo = ammo2;
			return;
		}
	}
}


void	CLocalPlayer::ServerUpdateWeapon()
{
	g_CCore->GetLog()->AddLog("CLocalPlayer::ServerUpdateWeapon", LOG_ALL);
	PED* player = (PED*) this->GetEntity();
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SWITCHWEAPON);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	bsOut.Write(player->inventary.slot[0].weaponType);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}


void	CLocalPlayer::OnDeath(int killer, int reason, int part)
{
	char buff[250];
	sprintf(buff, "Attacker: %d Reason %d Part %d", killer, reason, part);
	g_CCore->GetChat()->AddMessage(buff);

	// Send info to server
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_DEATH);
	// killer ID
	bsOut.Write(killer);
	// reason type (shot)
	bsOut.Write(reason);
	// player's part type (head, body,leg)
	bsOut.Write(part);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}