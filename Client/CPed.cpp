#include "CPed.h"
#include "CCore.h"

extern CCore *g_CCore;
CPed::CPed()
{
	this->SetActive(true);
	isActive = true;
	this->SetIsOnFoot(true);
	sprintf(sName,"Player");
	playerPos.x = -1985.97f;
	playerPos.y = -5.03f;
	playerPos.z = 4.28f;

	actual = playerPos;
	previous = playerPos;
	isSpawned	= 0;
	isDucking	= 0;
	//pedBase		= NULL;
	//frame		= NULL;

	fHealth		= 200.0f;
	animId		= -1; // this mean that we dont use custom anim at the moment
	ping		= -1;
	skinId		= 0;

	//timeDiff = timeGetTime();
	//timeLastMessage = timeDiff;

	currentWep = 0;
	for(int i = 0; i < 8;i++)
		this->weapon[i].wepID = 0;

	this->InCar = -1;

	this->nametag = NULL;
}

CPed::~CPed()
{
	if (this->nametag != NULL)
	{
		this->nametag->Release();
	}
}
void CPed::SetActive(bool b)
{
	isActive = b;
}


void CPed::SetCarAnim(bool b)
{
	this->isCarAnim = b;
}
bool CPed::IsCarAnim()
{
	return (this->isCarAnim == 1);
}

void CPed::SetIsOnFoot(bool b)
{
	this->isOnFoot = b;
}
bool CPed::IsOnFoot()
{
	return this->isOnFoot;
}
void CPed::SetPing(int p)
{
	ping = p;
}
/*void CPed::SetEntity(DWORD address)
{
	this->pedBase = address;
}
void CPed::SetPosition(Vector3D vect)
{
	playerPos = vect;
}*/
void CPed::SetName(char* nm)
{
	sprintf(sName,"%s",nm);
}
/*void CPed::SetHealth(float ht)
{
	fHealth = ht;
}
void CPed::SetRotation(float f1,float f2,float f3)
{
	rotation1 = f1;
	rotation2 = f2;
	rotation3 = f3;
}
void CPed::SetState(byte st)
{
	state = st;
}*/
char* CPed::GetName()
{
	return sName;
}
/*
float CPed::GetHealth()
{
	return fHealth;
}
byte CPed::GetState()
{
	return state;
}
*/
void CPed::SetDucking(byte ducking)
{
	isDucking = ducking;
}
bool CPed::IsActive()
{
	return isActive;
}
byte CPed::IsDucking()
{
	return isDucking;
}
/*
Vector3D CPed::GetPosition()
{
	return playerPos;
}
bool CPed::HasBase()
{
	if(this->pedBase == 0)
		return false;
	return true;
}

void CPed::SetSkinId(int id)
{
	this->skinId = id;
}
int CPed::GetSkin()
{
	return this->skinId;
}*/

int CPed::GetPing()
{
	return ping;
}

void CPed::UpdateGameObject()
{
	PED* ped = (PED*) this->GetEntity();
	if (ped != NULL)
	{
		ped->isInAnimWithCar = this->IsCarAnim();
		if (ped->health > 0.0f)
		{
			if (this->IsOnFoot() && ped->playersCar == NULL && (this->IsCarAnim() == false))
			{
				//Vector3D rot = this->GetRotation();
				//ped->object.rotation = rot;

				ped->health = this->fHealth;
				if (this->state != 163)
					ped->animState = this->state;
				ped->isDucking = this->isDucking == 1;
				if (currentWep != 0)
					ped->isAiming = this->isAiming == 1;
				else
					ped->isAiming = 0;

				/**(float*)(this->EntityBase + 0x644) = this->fHealth;
				if (this->state != 163)
				{
					*(byte*)(this->EntityBase + 0x74) = this->state;
				}
				*(byte*)(this->EntityBase + 0x1E4) = this->isDucking;
				if (currentWep != 0)
					*(byte*)(this->EntityBase + 0x1E5) = this->isAiming;
				else
					*(byte*)(this->EntityBase + 0x1E5) = 0;*/

			}
		}
	}
}

void CPed::SetAnim(int id)
{
	this->animId = id;
}
int CPed::GetAnim()
{
	return this->animId;
}

void CPed::SetUpInterpolation()
{
	//char buf[255];
	//sprintf(buf, "%i", this->timestamp);
	/*this->previous = this->actual;
	this->actual = this->playerPos;

	this->timeDiff = timestamp - timeLastMessage;
	this->timeLastMessage = timestamp;
	interpolationTick = RakNet::GetTimeMS();
	*/
	/*interpolationGenerated = interpolationTick;

	Vector3D speedvector;
	speedvector.x = this->actual.x - this->previous.x;
	speedvector.y = this->actual.y - this->previous.y;
	speedvector.z = this->actual.z - this->previous.z;
	Vector3D lagDiff;
	float	coef = (float)(30.0f / (float)this->timeDiff);
	lagDiff.x =(this->actual.x + (speedvector.x*coef)-;
	lagDiff.y = this->actual.x + (speedvector.x*coef);
	lagDiff.z = this->actual.x + (speedvector.x*coef);
	*/
	/*// fix gap (some time spent after position was generated on another client side)
	float coeficient = (float)(RakNet::GetTime() - timeLastMessage) / (float) timeDiff;
	Vector3D speedvector;
	speedvector.x = this->actual.x - this->previous.x;
	speedvector.y = this->actual.y - this->previous.y;
	speedvector.z = this->actual.z - this->previous.z;
	// multiply speedvector by elapsed time
	speedvector.x *= coeficient;
	speedvector.y *= coeficient;
	speedvector.z *= coeficient;
	// now sum it with start position
	this->playerPos.x += speedvector.x;
	this->playerPos.y += speedvector.y;
	this->playerPos.z += speedvector.z;*/
	interpolation.SetUpInterpolation(playerPos);
	interpolation.SetUpInterpolationRot(rotation);
}

void CPed::SetAiming(byte bIs)
{
	this->isAiming = bIs;
}

void CPed::Interpolate()
{
	// this func should be called in Endscene
	// hardcoded by Romop5, 25.12 at 0:10 AM :D :D :D

	if (this->GetEntity() != 0)
	{
		PED* ped = (PED*) this->GetEntity();
		//if (ped->object.isActive == 1)
		//	return;
		//RakNet::TimeMS actualtime = RakNet::GetTimeMS(), b = this->timestamp, c = this->timeDiff;
		//char buff[255];
		//sprintf(buff, "I: %i %i %i", actualtime, b, c);
		//g_CCore->GetLog()->AddLog(buff);
		if (this->fHealth == 0) return;
		DWORD car = *(DWORD*)(this->EntityBase + 0x98);
		if (this->IsOnFoot() && car == NULL)
		{

			/*float coeficient = 0;
			if (c != 0)
			{
				coeficient = (FLOAT)((FLOAT)(actualtime - interpolationTick) / (FLOAT)c);
			}
			/*char buff[255];
			sprintf(buff, "I: %f", coeficient);
			g_CCore->GetLog()->AddLog(buff);
			*/
			/*Vector3D speedvector;
			speedvector.x = this->actual.x - this->previous.x;
			speedvector.y = this->actual.y - this->previous.y;
			speedvector.z = this->actual.z - this->previous.z;
			/*char buff[255];
			sprintf(buff, "I: %f %f %f", speedvector.x ,speedvector.y, speedvector.z);
			g_CCore->GetLog()->AddLog(buff);*/
			
			// multiply speedvector by elapsed time
			/*speedvector.x *= coeficient;
			speedvector.y *= coeficient;
			speedvector.z *= coeficient;
			// now sum it with start position
			this->playerPos.x += speedvector.x;
			this->playerPos.y += speedvector.y;
			this->playerPos.z += speedvector.z;

			*(float*)(this->EntityBase + 0x24) = this->playerPos.x;
			*(float*)(this->EntityBase + 0x28) = this->playerPos.y;
			*(float*)(this->EntityBase + 0x2C) = this->playerPos.z;
			/*
			Vector3D rot = this->GetRotation();
			*(float*)(this->EntityBase + 0x30) = rot.x;
			*(float*)(this->EntityBase + 0x38) = rot.z;
			//*(float*)(this->EntityBase + 0x238) = this->rotation3;

			*(float*)(this->EntityBase + 0x644) = this->fHealth;
			*(byte*)(this->EntityBase + 0x74) = this->state;
			*(byte*)(this->EntityBase + 0x1E4) = this->isDucking;
			if (currentWep != 0)
				*(byte*)(this->EntityBase + 0x1E5) = this->isAiming;
			else
				*(byte*)(this->EntityBase + 0x1E5) = 0;*/
		//}
			// write it to ped
			/**(float*) (this->pedBase+0x24) = this->playerPos.x;
			*(float*) (this->pedBase+0x28) = this->playerPos.y;
			*(float*) (this->pedBase+0x2C) = this->playerPos.z;*/
			//interpolationTick = actualtime;
			PED* ped = (PED*) this->GetEntity();

			//this->playerPos = interpolation.Interpolate();
			
			/*
			Enable this if scripting commands break, caused some visual sync problems though
			this->playerPos = interpolation.Interpolate();
			this->rotation = interpolation.InterpolateRot();

			ped->object.position = this->playerPos;
			ped->object.rotation = this->rotation;*/

			ped->object.position = interpolation.Interpolate();
			ped->object.rotation = interpolation.InterpolateRot();

			//this->SetRotation(ped->object.rotation);

			/**(float*)(this->EntityBase + 0x24) = this->playerPos.x;
			*(float*)(this->EntityBase + 0x28) = this->playerPos.y;
			*(float*)(this->EntityBase + 0x2C) = this->playerPos.z;*/
		}
	}
}

// Weapons
int CPed::GetCurrentWeapon()
{
	return this->currentWep;
}
void CPed::SetCurrentWeapon(int ID)
{
	this->currentWep = ID;
}

void CPed::AddWeapon(int ID, int Loaded, int Hidden)
{
	if (this->currentWep == 0)
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
	for (int i = 0; i < 8; i++)
	{
		if (weapon[i].wepID == NULL)
		{
			weapon[i].wepID = ID;
			weapon[i].wepLoaded = Loaded;
			weapon[i].wepHidden = Hidden;
			break;
		}
	}
}

void CPed::DeleteWeapon(int ID)
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

void CPed::SwitchWeapon(int ID)
{
	currentWep = ID;
}

void CPed::OnShoot()
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

SWeapon* CPed::GetWeapon(int index)
{
	return &weapon[index];
}

Vector3D CPed::GetPosition()
{
	if (this->InCar == -1)
	{
		return this->playerPos;
	}
	else
	{
		//CVehicle* veh = g_CCore->GetVehiclePool()->Return(this->InCar);
		//return veh->GetPosition();
		if (this->EntityBase != NULL)
		{
			DWORD base_ped = this->EntityBase;
			DWORD base = *(DWORD*)(base_ped + 0x68);
			Vector3D pos;
			pos.x = *(float*)(base + 0x40);
			pos.y = *(float*)(base + 0x44);
			pos.z = *(float*)(base + 0x48);
			return pos;
		}
		return this->playerPos;
	}
}


Vector3D	CPed::GetPEDFramePosition()
{
	if (this->EntityBase != NULL)
	{
		DWORD base_ped = this->EntityBase;
		DWORD base = *(DWORD*)(base_ped + 0x68);
		Vector3D pos;
		pos.x = *(float*)(base + 0x40);
		pos.y = *(float*)(base + 0x44);
		pos.z = *(float*)(base + 0x48);
		return pos;
	}
	return Vector3D();
}

void CPed::OnThrowGranade()
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


void CPed::ClearWeapons()
{
	for (int i = 0; i < 8; i++)
	{
		this->weapon[i].wepID = 0;
		this->weapon[i].wepLoaded = 0;
		this->weapon[i].wepHidden = 0;
	}
}

void CPed::gCheckWeapons()
{
	PED* ped = (PED*) this->GetEntity();
	if (ped)
	{
		if (ped->inventary.slot[0].weaponType != this->currentWep)
		{
			if (ped->inventary.slot[0].weaponType == 0)
			{
				g_CCore->GetGame()->AddWeapon(this->GetEntity(), this->currentWep, 1000, 0, 0);
			}
			else {
				g_CCore->GetGame()->DeleteWeapon(this->GetEntity(), ped->inventary.slot[0].weaponType);
				g_CCore->GetGame()->AddWeapon(this->GetEntity(), this->currentWep, 1000, 0, 0);
			}
		}
		else {
			ped->inventary.slot[0].ammoLoaded = 1000;
		}
	}
}