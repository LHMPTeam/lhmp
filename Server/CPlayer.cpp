#include "CCore.h"

#include "CPlayer.h"
#include "../shared/tools.h"

extern CCore *g_CCore;

CPlayer::CPlayer()
{
	this->InCar		= -1;
	this->isActive  = 1;
	this->SetNickname("Player");
	this->health	= 200.0f;
	this->isSpawned = 1;

	this->currentWep = 0;
	for(int i = 0; i < 8;i++)
		this->weapon[i].wepID = 0;
	this->money = 0;
	this->fov = 0;
	this->isLocked = false;
}

CPlayer::~CPlayer()
{
}

char* CPlayer::GetNickname()
{
	return this->nickname;
}

int CPlayer::GetCurrentWeapon()
{
	return this->currentWep;
}
SWeapon* CPlayer::GetWeapon(int index)
{
	return &weapon[index];
}

bool CPlayer::IsAim()
{
	return this->isAim;
}
bool CPlayer::IsDucking()
{
	return this->isDucking;
}

bool CPlayer::IsActive()
{
	return this->isActive;
}

bool	 CPlayer::IsCarAnim()
{
	return this->isCarAnim;
}

void CPlayer::SetMoney(int money)
{
	this->money = money;
}
void CPlayer::SetIsAim(bool aim)
{
	this->isAim = aim;
}

void CPlayer::SetIsDucking(bool is)
{
	this->isDucking = is;
}
void CPlayer::SetIsActive(bool is)
{
	this->isActive = is;
}

void CPlayer::SetNickname(char nick[])
{
	sprintf(this->nickname,"%s",nick);
}
void CPlayer::SetNickname(const char* nick)
{
	sprintf(this->nickname,"%s",nick);
}

void CPlayer::SetCurrentWeapon(int ID)
{
	this->currentWep = ID;
}

void CPlayer::AddWeapon(int ID, int Loaded,int Hidden)
{
	if(this->currentWep == 0)
		currentWep = ID;
	// If we already have that weapon && it must be a shot gun
	if (Tools::isShootingArm(ID))
	{
		for (int i = 0; i < 8; i++)
		{
			if (weapon[i].wepID == ID)
			{
				weapon[i].wepHidden += Hidden;
				return;
			}
		}
	}
	// else find first empty slot
	for(int i = 0; i < 8;i++)
	{
		if(weapon[i].wepID == NULL)
		{
			weapon[i].wepID			= ID;
			weapon[i].wepLoaded		= Loaded;
			weapon[i].wepHidden		= Hidden;
			break;
		}
	}
}

void CPlayer::DeleteWeapon(int ID)
{
	if(currentWep == ID)
		currentWep = 0;
	for(int i = 0; i < 8;i++)
	{
		if(weapon[i].wepID == ID)
		{
			weapon[i].wepID			= 0;
			weapon[i].wepLoaded		= 0;
			weapon[i].wepHidden		= 0;
			break;
		}
	}
}

void CPlayer::SwitchWeapon(int ID)
{
	currentWep = ID;
}

void CPlayer::OnShoot()
{
	for(int i = 0; i < 8;i++)
	{
		if(weapon[i].wepID == currentWep)
		{
			if(weapon[i].wepLoaded == 0)
			{
				// TODO: reload podla poctu nabojov v zasobniku
				weapon[i].wepLoaded = weapon[i].wepHidden;
				weapon[i].wepHidden = 0;
			} else
			{
				weapon[i].wepLoaded--;
			}
			break;
		}
	}
}

void CPlayer::ResetWeapons()
{
	for(int i = 0; i < 8;i++)
	{
		this->weapon[i].wepID = 0;
		this->weapon[i].wepLoaded = 0;
		this->weapon[i].wepHidden = 0;
	}
	this->currentWep = 0;
}

void CPlayer::SetCarAim(float f)
{
	this->carAim = f;
}

void CPlayer::SetFOV(float r)
{
	this->fov = r;
}


void CPlayer::SetIsCarAnim(bool b)
{
	this->isCarAnim = b;
}

float CPlayer::GetFOV()
{
	return this->fov;
}

float CPlayer::GetCarAim()
{
	return this->carAim;
}

int CPlayer::GetMoney()
{
	return this->money;
}

void CPlayer::OnThrowGranade()
{
	for (int i = 0; i < 8; i++)
	{
		if (weapon[i].wepID == currentWep)
		{
			weapon[i].wepID = 0;
			weapon[i].wepLoaded = 0;
			weapon[i].wepHidden = 0;
			break;
		}
	}
	currentWep = 0;
}


bool	CPlayer::IsLocked()
{
	return this->isLocked;
}
void	CPlayer::SetLocked(bool status)
{
	this->isLocked = status;
}


//------------ Network


void	CPlayer::OnSync(SYNC::ON_FOOT_SYNC syncData, RakNet::TimeMS time)
{
	this->SetPosition(syncData.position);
	Vector3D rot;
	rot.x = syncData.degree;
	rot.z = syncData.degree_second;
	this->SetRotation(rot);
	this->SetStatus(syncData.status);
	this->SetHealth(syncData.health);
	this->SetIsDucking(syncData.isDucking);
	this->SetIsAim(syncData.isAim);
	this->SetIsCarAnim(syncData.isCarAnim);
	this->SetTimeStamp(time);

	g_CCore->GetPickupPool()->TestPlayer(g_CCore->GetPlayerPool()->GetID(this));
}


void	CPlayer::OnCarUpdate(SYNC::IN_CAR syncData)
{
	this->SetHealth(syncData.health);
	this->SetIsAim(syncData.isAiming);
	this->SetCarAim(syncData.aim);
	g_CCore->GetPickupPool()->TestPlayer(g_CCore->GetPlayerPool()->GetID(this));
}

void	CPlayer::OnChangeSkin(int skin)
{
	if (skin > 302)
		return;
	this->SetSkin(skin);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_CHANGESKIN);
	bsOut.Write(g_CCore->GetPlayerPool()->GetID(this));
	bsOut.Write(skin);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
}

void	CPlayer::OnRespawn()
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->ResetWeapons();
	if (this->InCar != -1)
	{
		g_CCore->GetVehiclePool()->Return(this->InCar)->PlayerExit(ID);
	}

	g_CCore->GetNetworkManager()->SendHimDoors(ID);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_RESPAWN);
	bsOut.Write(ID);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);
	g_CCore->GetScripts()->onPlayerSpawn(ID);
}


void	CPlayer::OnAddWeapon(int wepID, int ammoA, int ammoB, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->AddWeapon(wepID,ammoA,ammoB);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_ADDWEAPON);
	bsOut.Write(ID);
	bsOut.Write(wepID);
	bsOut.Write(ammoA);
	bsOut.Write(ammoB);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS,true);	// send this message everybody including this player - when using Squirrel
	else 
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
}

void	CPlayer::OnDeleteWeapon(int wepID, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->DeleteWeapon(wepID);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_DELETEWEAPON);
	bsOut.Write(ID);
	bsOut.Write(wepID);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
}

void	CPlayer::OnSwitchWeapon(int wepID, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->SwitchWeapon(wepID);

	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_SWITCHWEAPON);
	bsOut.Write(ID);
	bsOut.Write(wepID);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
}

void	CPlayer::OnPlayerShoot(float x,float y,float z, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);
	this->OnShoot();
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_SHOOT);
	bsOut.Write(ID);
	bsOut.Write(x);
	bsOut.Write(y);
	bsOut.Write(z);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
	g_CCore->GetScripts()->onPlayerShoot(ID, this->GetCurrentWeapon());
}

void	CPlayer::OnPlayerThrowGranade(Vector3D position, bool shouldSendHim)
{
	int ID = g_CCore->GetPlayerPool()->GetID(this);


	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_THROWGRANADE);
	bsOut.Write(ID);
	bsOut.Write(position);
	if (shouldSendHim)
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);	// send this message everybody including this player - when using Squirrel
	else
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), true);	// else, when it arrives from player
	
	this->OnThrowGranade();
	g_CCore->GetScripts()->onPlayerShoot(ID, this->GetCurrentWeapon());
}