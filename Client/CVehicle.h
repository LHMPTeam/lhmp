/**
	Lost Heaven Multiplayer

	Purpose: vehicle base class

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code
*/

#ifndef __VEHICLE_H
#define	__VEHICLE_H
#include "CEntity.h"
#include "../shared/structures.h"
class CVehicle : public CEntity
{
private:
	byte			isSpawned;
	bool			isStarted;

	// interpolation
	Vector3D		actual, previous;
	RakNet::TimeMS	timeLastMessage;
	RakNet::TimeMS	timeDiff;
	RakNet::TimeMS	idleStartTime;
	Vector3D		interpolationVector; // metres per ms
	Vector3D		tempVector;

	// slots
	int				Seat[4];

	Vector3D		speed;
	float			wheels;
	float			fuel;

	Vector3D		vehPos;
	bool			horn;
	bool            siren;
	Vector3D		secondRot;

	float			damage;
	byte			shotdamage;

	bool			onGas;	// is accelerating
	byte            roofState;
	byte            engineState;

	DWORD			explodedCar;




public:
	//DWORD		pedBase;
	//DWORD		frame;
	CVehicle();
	~CVehicle();
	void		UpdateGameObject();

	void		SetName(char[]);
	void		SetActive(bool);

	bool		IsActive();
	char*		GetName();
	void		SetUpInterpolation();
	void		Interpolate();

	void		SendSync();
	void		PlayerEnter(int, int);
	void		PlayerExit(int);

	void		SetSpeed(Vector3D);
	Vector3D	GetSpeed();

	void		SetWheelsAngle(float);
	float		GetWheelsAngle();

	int			GetSeat(int);
	void		SetSeat(int, int);

	void		SetVehiclePosition(Vector3D);
	Vector3D	GetVehiclePosition();

	void		SetHornState(bool);
	bool		GetHornState();

	void		SetFuel(float);
	float		GetFuel();

	void        SetSirenState(bool);
	bool        GetSirenState();

	void		SetSecondRot(Vector3D);
	Vector3D	GetSecondRot();

	Vector3D	GetTempVect();
	Vector3D	GetInterVect();

	float		GetDamage();
	void		SetDamage(float);

	byte		GetShotDamage();
	void		SetShotDamage(byte);

	void        ToggleRoof(byte);
	byte        GetRoofState();

	void        ToggleEngine(byte);
	byte        GetEngineState();

	bool		IsOnGas();
	void		SetIsOnGas(bool);

	int			GetPlayerSeat(int);

	DWORD		GetExplodedCar();
	void		SetExplodedCar(DWORD);
};
#endif