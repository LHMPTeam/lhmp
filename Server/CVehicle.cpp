#include "CVehicle.h"
#include "CCore.h"
extern CCore *g_CCore;

CVehicle::CVehicle()
{
	for (int i = 0; i < 4; i++)
		Seat[i] = -1;
	speed.x = 0.0f;
	speed.y = 0.0f;
	speed.z = 0.0f;
	horn = 0;

	secondRot.x = 0.0f;
	secondRot.y = 0.0f;
	secondRot.z = 0.0f;
	this->SetRespawnRotation(secondRot);
	damage = 100.0f;
	shotdamage = 10;
	onGas = 0; 
	roofState = 0;
	engineState = 0;
	siren = 0;
	isExploded = false;
}
CVehicle::~CVehicle()
{
	for (int i = 0; i < 4; i++)
		this->PlayerExit(i);
}

void CVehicle::ToggleRoof(byte state)
{
	this->roofState = state;
}

byte CVehicle::GetRoofState()
{
	return this->roofState;
}

void CVehicle::ToggleEngine(byte state)
{
	this->engineState = state;
}

byte CVehicle::GetEngineState()
{
	return this->engineState;
}

bool CVehicle::IsStarted()
{
	return this->isStarted;
}
void CVehicle::SetStarted(bool b)
{
	this->isStarted = b;
}

void CVehicle::SetSpeed(Vector3D speed)
{
	this->speed = speed;
}
Vector3D CVehicle::GetSpeed()
{
	return this->speed;
}

void CVehicle::PlayerEnter(int pID, int seatID)
{
	this->Seat[seatID] = pID;
	CPlayer* player = g_CCore->GetPlayerPool()->Return(pID);
	if (player != NULL)
	{
		player->InCar = g_CCore->GetVehiclePool()->ReturnId(this);
	}
}
void CVehicle::PlayerExit(int pID)
{
	if (pID == 0)
	{
		Vector3D speed;
		speed.x = 0; speed.y = 0; speed.z = 0;
		this->SetSpeed(speed);
		this->onGas = false;
	}
	for (int i = 0; i < 4;i++)
		if (Seat[i] == pID)
			Seat[i] = -1;
	CPlayer* player = g_CCore->GetPlayerPool()->Return(pID);
	if (player != NULL)
	{
		player->InCar = -1;
	}
}

void CVehicle::SendSync()
{
	if (this->isExploded)
	{
		unsigned int elapsedTime = g_CCore->GetTickManager()->GetTime() - this->GetExplodeTime();
		if (elapsedTime > 1000)
		{
			this->Respawn();
		}
	}
	else {
		VEH::SYNC syncData;
		syncData.ID = g_CCore->GetVehiclePool()->ReturnId(this);
		syncData.position = this->GetPosition();
		syncData.rotation = this->GetRotation();
		syncData.speed = this->GetSpeed();
		syncData.wheels = this->GetWheelsAngle();
		syncData.horn = this->GetHornState();
		syncData.gasOn = this->IsOnGas();
		syncData.secondRot = this->GetSecondRot();
		//std::cout << " SS2 " << syncData.ID << std::endl;
		BitStream bsOut;
		bsOut.Write((MessageID)ID_TIMESTAMP);
		if (this->GetSeat(0) != -1)
			bsOut.Write(this->GetTimeStamp());
		else
			bsOut.Write(RakNet::GetTimeMS());
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_SYNC);
		bsOut.Write(syncData);
		if (Seat[0] == -1)
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, LOW_PRIORITY, UNRELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
		else
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, LOW_PRIORITY, UNRELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(Seat[0]), true);
	}
}

void	CVehicle::SetWheelsAngle(float w)
{
	this->wheels = w;
}
float	CVehicle::GetWheelsAngle()
{
	return this->wheels;
}


int	 CVehicle::GetSeat(int ID)
{
	return this->Seat[ID];
}
void CVehicle::PlayerDisconnect(int ID)
{
	if (ID == 0)
	{
		Vector3D speed;
		speed.x = 0; speed.y = 0; speed.z = 0;
		this->SetSpeed(speed);
		this->onGas = false;
	}
	for (int i = 0; i < 4; i++)
	{
		if (this->Seat[i] == ID)
			Seat[i] = -1;
	}
}

void CVehicle::SetHornState(bool b)
{
	this->horn = b;
}
bool CVehicle::GetHornState()
{
	return this->horn;
}

void CVehicle::SetSirenState(bool b)
{
	this->siren = b;
}

bool CVehicle::GetSirenState()
{
	return this->siren;
}

void CVehicle::SetFuel(float fuel)
{
	this->fuel = fuel;
}

float CVehicle::GetFuel()
{
	return this->fuel;
}

void CVehicle::SetSecondRot(Vector3D rot)
{
	this->secondRot = rot;
}
Vector3D CVehicle::GetSecondRot()
{
	return this->secondRot;
}

float	CVehicle::GetDamage()
{
	return this->damage;
}
void	CVehicle::SetDamage(float dmg)
{
	this->damage = dmg;
}
byte	CVehicle::GetShotDamage()
{
	return this->shotdamage;
}
void	CVehicle::SetShotDamage(byte dmg)
{
	this->shotdamage = dmg;
}

bool CVehicle::IsOnGas()
{
	return this->onGas;
}
void	CVehicle::SetOnGas(bool g)
{
	this->onGas = g;
}

bool	CVehicle::IsExploded()
{
	return this->isExploded;
}
void	CVehicle::SetExploded(bool exp)
{
	this->isExploded = exp;
	this->SetIsSpawned(!exp);
}

void		CVehicle::SetRespawnPosition(Vector3D position)
{
	this->respawnPosition = position;
}
Vector3D	CVehicle::GetRespawnPosition()
{
	return this->respawnPosition;
}

void		CVehicle::SetRespawnRotation(Vector3D position)
{
	this->respawnRotation = position;
}
Vector3D	CVehicle::GetRespawnRotation()
{
	return this->respawnRotation;
}

void		CVehicle::SetRespawnRotationSecond(Vector3D position)
{
	this->respawnRotationSecond = position;
}
Vector3D	CVehicle::GetRespawnRotationSecond()
{
	return this->respawnRotationSecond;
}

unsigned int CVehicle::GetExplodeTime()
{
	return this->explodeTime;
}
void		CVehicle::SetExplodeTime(unsigned int time)
{
	this->explodeTime = time;
}

void	CVehicle::Respawn()
{
	g_CCore->GetLog()->AddNormalLog("Car respawn");
	this->rotation = this->GetRespawnRotation();
	this->position = this->GetRespawnPosition();
	this->secondRot = this->GetRespawnRotationSecond();
	this->health = 100.0f;
	this->damage = 100.0f;
	this->shotdamage = 10;
	this->isExploded = false;
	Vector3D speed;
	speed.x = 0.0f;
	speed.y = 0.0f;
	speed.z = 0.0f;
	this->SetSpeed(speed);

	RakNet::BitStream bsOut;
	bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_RESPAWN);
	bsOut.Write(g_CCore->GetVehiclePool()->ReturnId(this));
	bsOut.Write(this->position);
	bsOut.Write(this->rotation);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_RAKNET_GUID, true);

	this->SetIsSpawned(true);
}