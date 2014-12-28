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
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->inCarRotation;
	//return *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x5F4);
}
float CLocalPlayer::GetHealth()
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->health;
	//return *(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x644);
}
byte CLocalPlayer::GetStatus()
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->animState;
	//return	*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x70);
}
bool CLocalPlayer::GetIsAiming()
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->isAiming;
	//return	(*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x1E5) == 1);
}
bool CLocalPlayer::GetIsDucking()
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->isDucking;
	//return	(*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x1E4) == 1);
}

bool CLocalPlayer::GetIsCarAnim()
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	return ped->isInAnimWithCar;
	//return	(*(BYTE*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x71) == 1);
}
bool CLocalPlayer::IsOnFoot()
{
		return bIsOnFoot;
}
void CLocalPlayer::SetIsOnFoot(bool b)
{
	this->bIsOnFoot = b;
	if (b)
		g_CCore->GetLog()->AddLog("setisonfoot 1");
	else
		g_CCore->GetLog()->AddLog("setisonfoot 0");
}

void CLocalPlayer::Pulse()
{
	if (this->GetEntity() != NULL)
	{
		if (this->IsOnFoot())
		{
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

			/*char buff[255];
			sprintf(buff, "STATE %x", this->GetStatus());
			g_CCore->GetLog()->AddLog(buff);
			*/
		}
		else
		{
			//MessageBox(NULL,"aha","aha",MB_OK);
			//g_CCore->GetLog()->AddLog("InCar");
			//DWORD car = ((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4) + 0x3088));
			//int ID = g_CCore->GetVehiclePool()->GetVehicleIdByBase(car);
			if (this->IDinCar != -1)
			{
				CVehicle* veh = g_CCore->GetVehiclePool()->Return(IDinCar);
				if (veh != NULL)
					veh->SendSync();
			}
			//g_CCore->GetLog()->AddLog("Auto");

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
	this->ourID = id;
}

int CLocalPlayer::GetOurID()
{
	return this->ourID;
}

Vector3D CLocalPlayer::GetLocalPos()
{
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
	return nickname;
}
void	CLocalPlayer::SetNickname(char* nick)
{
	sprintf(nickname,"%s",nick);
}

void	CLocalPlayer::SetPing(int p)
{
	ping = p;
}

int		CLocalPlayer::GetPing()
{
	return ping;
}

DWORD CLocalPlayer::GetBase()
{
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
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	ped->object.rotation = rot;
	/**(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x30) = rot.x;
	*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x34) = rot.y;
	*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x38) = rot.z;*/
}

void	CLocalPlayer::SetHealth(float f)
{
	PED* ped = g_CCore->GetGame()->GetLocalPED();
	ped->health = f;
	//*(float*)((*(DWORD*)((*(DWORD*)0x006F9464) + 0xE4)) + 0x644) = f;

	// ingame GUI change
	(*(DWORD*)0x00661538)	= (unsigned int)(f/2);
}


bool	CLocalPlayer::HasWeapon(int wepID)
{
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
	PED* player = (PED*) this->GetEntity();
	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_PLAYER_SWITCHWEAPON);
	bsOut.Write(g_CCore->GetLocalPlayer()->GetOurID());
	bsOut.Write(player->inventary.slot[0].weaponType);
	g_CCore->GetNetwork()->SendServerMessage(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED);
}