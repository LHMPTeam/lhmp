#ifndef VEHICLE_H
#define	VEHICLE_H
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
	Vector3D		interpolationVector; // metres per ms
	Vector3D		tempVector;

	// slots
	int				Seat[4];

	Vector3D		speed;
	float			wheels;

	Vector3D		vehPos;
	bool			horn;
	Vector3D		secondRot;

	float			damage;
	byte			shotdamage;

	bool			onGas;	// is accelerating
	byte            roofState;

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

	bool		IsOnGas();
	void		SetIsOnGas(bool);

	int			GetPlayerSeat(int);

	DWORD		GetExplodedCar();
	void		SetExplodedCar(DWORD);
};
#endif