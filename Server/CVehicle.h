// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef CVEHICLE_H
#define CVEHICLE_H
#include "../shared/structures.h"
#include "CEntity.h"
class CVehicle : public CEntity
{
private:
	char			nickname[255];
	bool			isStarted;

	Vector3D		speed;
	float			wheels;
	// slots
	int				Seat[4];
	bool			horn;
	bool			siren;
	Vector3D		secondRot;

	float			damage;
	float			fuel;
	byte			shotdamage;

	bool			onGas;
	byte            roofState;    
	byte            engineState;

	bool			isExploded;

	Vector3D		respawnPosition;
	Vector3D		respawnRotation;
	Vector3D		respawnRotationSecond;

	unsigned int	explodeTime;

public:
	CVehicle();
	~CVehicle();

	bool IsStarted();
	void SetStarted(bool);
	void		SetSpeed(Vector3D);
	Vector3D	GetSpeed();
	void		SetWheelsAngle(float);
	float		GetWheelsAngle();

	void		PlayerEnter(int, int);
	void		PlayerExit(int);

	void		SendSync();
	int			GetSeat(int);
	void		PlayerDisconnect(int);

	void		SetHornState(bool);
	bool		GetHornState();

	void		SetFuel(float);
	float		GetFuel();

	void        SetSirenState(bool);
	bool        GetSirenState();

	void		SetSecondRot(Vector3D);
	Vector3D	GetSecondRot();

	float		GetDamage();
	void		SetDamage(float);

	byte		GetShotDamage();
	void		SetShotDamage(byte);

	void        ToggleRoof(byte);
	byte        GetRoofState();

	void        ToggleEngine(byte);
	byte        GetEngineState();

	bool		IsOnGas();
	void		SetOnGas(bool);

	bool		IsExploded();
	void		SetExploded(bool);

	void		SetRespawnPosition(Vector3D);
	Vector3D	GetRespawnPosition();

	void		SetRespawnRotation(Vector3D);
	Vector3D	GetRespawnRotation();

	void		SetRespawnRotationSecond(Vector3D);
	Vector3D	GetRespawnRotationSecond();


	unsigned int GetExplodeTime();
	void		SetExplodeTime(unsigned int);
	void		Respawn();
};
#endif