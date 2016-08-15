// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CEntity.h"

CEntity::CEntity()
{
	this->isActive		= 1;
	this->health		= 200.0f;
	this->isSpawned		= 1;
	this->shouldUpdate	= true;
	this->timestamp		= NULL;
	this->skinID		= 0;
	this->status		= 0;

}

CEntity::~CEntity()
{
}

float CEntity::GetHealth()
{
	return this->health;
}

Vector3D CEntity::GetPosition()
{
	return this->position;
}

Vector3D CEntity::GetRotation()
{
	return this->rotation;
}
/*float CEntity::GetRotationSecond()
{
	return this->degree_second;
}*/

int CEntity::GetSkin()
{
	return this->skinID;
}
byte CEntity::GetStatus()
{
	return this->status;
}

bool CEntity::IsSpawned()
{
	return this->isSpawned;
}
bool CEntity::IsActive()
{
	return this->isActive;
}

void CEntity::SetHealth(float health)
{
	this->health = health;
}

void CEntity::SetIsActive(bool is)
{
	this->isActive = is;
}

void CEntity::SetIsSpawned(bool is)
{
	this->isSpawned = is;
}
void CEntity::SetPosition(Vector3D pos)
{
	this->position = pos;
}

void CEntity::SetRotation(Vector3D rot)
{
	/*this->degree = r1;
	this->degree_second = r2;*/
	this->rotation = rot;
}

void CEntity::SetSkin(int ID)
{
	this->skinID = ID;
}

void CEntity::SetStatus(byte state)
{
	this->status = state;
}

void CEntity::SetTimeStamp(RakNet::TimeMS ts)
{
	this->timestamp = ts;
	this->shouldUpdate = true;
}
RakNet::TimeMS CEntity::GetTimeStamp()
{
	return this->timestamp;
}

bool CEntity::ShouldUpdate()
{
	return this->shouldUpdate;
}


void CEntity::SetShouldUpdate(bool b)
{
	this->shouldUpdate = b;
}