// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef ENTITY_H
#define ENTITY_H

#include "../shared/structures.h"

class CEntity
{
protected:
	bool			isActive;

	Vector3D		position;
	Vector3D		rotation;
	float			health;
	byte			status;
	int				skinID;
	RakNet::TimeMS	synctime;

	// bools
	bool			isSpawned;
	RakNet::TimeMS	timestamp;
	bool			shouldUpdate;
public:
	CEntity();
	~CEntity();

	void SetPosition(Vector3D);
	virtual void SetRotation(Vector3D);
	void SetHealth(float);
	void SetStatus(byte);
	void SetSkin(int);
	void SetIsSpawned(bool);
	void SetIsActive(bool);

	Vector3D		GetPosition();
	virtual  Vector3D		GetRotation();
	//float	GetRotationSecond();
	float			GetHealth();
	byte			GetStatus();
	int				GetSkin();
	bool			IsSpawned();
	bool			IsDucking();
	bool			IsActive();

	void			SetTimeStamp(RakNet::TimeMS);
	RakNet::TimeMS	GetTimeStamp();

	bool			ShouldUpdate();
	void			SetShouldUpdate(bool);
};
#endif