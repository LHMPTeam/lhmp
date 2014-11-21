#include "CEntity.h"
#include "CCore.h"

extern CCore *g_CCore;
CEntity::CEntity()
{
	playerPos.x = -1985.97f;
	playerPos.y = -5.03f;
	playerPos.z = 4.28f;
	isSpawned = 0;
	EntityBase = NULL;

	fHealth = 200.0f;
	skinId = 0;

	timestamp			= RakNet::GetTimeMS();
	//timeLastMessage		= RakNet::GetTimeMS();
	//timeDiff			= RakNet::GetTimeMS();
	//interpolationGenerated = RakNet::GetTimeMS();
	//interpolationCache = {0,0,0};
	shouldUpdate = 1;

	//bIsTempInterpolationRunning = true;
	//bIsInterpolationRunning		= true;
}
void CEntity::SetActive(bool b)
{
	isActive = b;
}

void CEntity::SetEntity(DWORD address)
{
	this->EntityBase = address;
}
DWORD CEntity::GetEntity()
{
	return this->EntityBase;
}
void CEntity::SetPosition(Vector3D vect)
{
	playerPos = vect;
}
void CEntity::SetHealth(float ht)
{
	fHealth = ht;
}
void CEntity::SetRotation(Vector3D rot)
{
	this->rotation = rot;
}
Vector3D CEntity::GetRotation()
{
	return this->rotation;
}
void CEntity::SetState(byte st)
{

	state = st;
}

float CEntity::GetHealth()
{
	return fHealth;
}
byte CEntity::GetState()
{
	return state;
}

bool CEntity::IsActive()
{
	return isActive;
}
Vector3D CEntity::GetPosition()
{
	return playerPos;
}


void CEntity::SetSkinId(int id)
{
	this->skinId = id;
}
int CEntity::GetSkin()
{
	return this->skinId;
}

void CEntity::SetTimeStamp(RakNet::TimeMS ts)
{
	this->timestamp = ts;
	this->shouldUpdate = true;
	interpolation.SetTimestamp(ts);
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

CInterpolation* CEntity::GetInterpolation()
{
	return &this->interpolation;
}