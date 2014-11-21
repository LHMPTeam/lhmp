#ifndef ENTITY_H
#define	ENTITY_H
#include "CInterpolation.h"
#include "../shared/structures.h"
class CEntity
{
protected:
	DWORD		EntityBase;
	bool		isActive;
	Vector3D	playerPos;
	Vector3D	rotation;
	float		fHealth;
	byte		state;
	byte		isSpawned;
	int			skinId;

	// interpolation
	// bools
	//bool			isSpawned;

	// interpolation
	RakNet::TimeMS	timestamp;
	/*RakNet::TimeMS	timeLastMessage;
	RakNet::TimeMS	timeDiff;
	RakNet::TimeMS	interpolationTick;*/
	//RakNet::TimeMS	interpolationGenerated;
	bool			shouldUpdate;
	//Vector3D		interpolationCache;
	bool			bIsTempInterpolationRunning;
	bool			bIsInterpolationRunning;
	RakNet::TimeMS	dwMsTempRun;

	CInterpolation	interpolation;


public:
	CEntity();
	//~CPed();
	DWORD		GetEntity();
	void		SetEntity(DWORD);

	//void		UpdateGameObject();
	void		SetPosition(Vector3D vect);
	void		SetName(char[]);
	void		SetHealth(float);
	void		SetRotation(Vector3D);
	Vector3D	GetRotation();
	void		SetState(byte);
	//void		SetDucking(byte);
	//void		SetAiming(byte);
	void		SetSkinId(int);
	//void		SetPing(int);
	void		SetActive(bool);

	bool		IsActive();
	//int			GetPing();
	char*		GetName();
	float		GetHealth();
	//void		SetRotation(float,float);
	byte		GetState();
	virtual	Vector3D	GetPosition();

	int			GetSkin();

	void			SetTimeStamp(RakNet::TimeMS);
	RakNet::TimeMS	GetTimeStamp();

	bool			ShouldUpdate();
	void			SetShouldUpdate(bool);

	virtual		void SetUpInterpolation() = 0;
	virtual		void Interpolate() = 0;

	CInterpolation* GetInterpolation();

};
#endif