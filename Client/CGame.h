/**
	Lost Heaven Multiplayer

	Purpose: class which handles all engine functions and callbacks

	@author Romop5, DavoSK, ZaKlaus
	@version 1.0 1/9/14
*/

#ifndef __CGAME_H
#define __CGAME_H

#include <stdio.h>
#include <string.h>
typedef unsigned long DWORD;
#include "../shared/structures.h"

enum ObjectTypes
{
	OBJECT_LOCALPLAYER = 0x2,
	OBJECT_VEHICLE = 0x4,
	OBJECT_ENEMY = 0x1B
};
#define _pad(x,y) byte x[y]

typedef struct _FRAME {
	_pad(_unk1, 0x100);		// 000-100
	char*	frameName;		// 100-104
	_pad(_unk2, 0x50);
	char*	frameModel;		// 154-158
} FRAME;
typedef struct _OBJECT {
	DWORD		vtable;				//  0000-0004
	_pad(__unk1,0xC);				//	0004-0010
	DWORD		objectType;			//	0010-0014
	_pad(__unk2, 0x10);				//	0014-0024
	Vector3D	position;			//	0024-0030
	Vector3D	rotation;			//	0030-003C
	_pad(__unk3, 0x21);				//  003C-005D
	bool		isActive;			//	005D-005E
	_pad(__unk4, 0xA);
	FRAME*		frame;				//	0068-006C
} OBJECT;
typedef struct _VEHICLE {
	OBJECT		object;				//  0000-006C
	_pad(__unk1, 0x238);
	float		engineDamage;		//	02A4-02A8	(0-100f)
	_pad(__unk2, 0x164);
	Vector3D	position;			//	040C-0418
	_pad(__unk3, 0x78);

	float		gasState;			//	0490-0494	(0/1.0f)
	_pad(__unk4, 0x88);
	bool		hornState;			//	051C-051D
	bool		sirenState;			//	051D-051E
	_pad(__unk5, 0x12A);

	int			speedGear;			//	0648-064C
	_pad(__unk6, 0xC4);
	float		wheelRotation;		//	0710-0714
	_pad(__unk7, 0x608);
	float		petrolLevel;		//	0D1C-0D20
	_pad(__unk8, 0x8);
	Vector3D	rotation;			//	0D28-0D34
	_pad(__unk9, 0xC);
	Vector3D	rotationSecond;		//	0D40-0D4C
	_pad(__unk10, 0x1230);
	Vector3D	speed;				//	1F7C-1F88
	_pad(__unk11, 0x10C);
	int			engineShotDamage;	//	2094-2098
} VEHICLE;

typedef struct _INVENTARY_SLOT {
	DWORD		weaponType;
	DWORD		ammoLoaded;
	DWORD		ammo;
	DWORD		__unk;		// maybe weaponObject
} INVETARY_SLOT;

typedef struct _INVENTARY {
	_INVENTARY_SLOT slot[8];
} INVENTARY;

typedef struct _PED {
	OBJECT		object;				//  0000-006C
	_pad(_unk0, 0x4);				//	006C-0070
	byte		animStateLocal;		//	0070-0071
	byte		isInAnimWithCarLocal;//	0071-0072
	_pad(_unk1, 0x2);				//	0072-0074
	byte		animState;			//	0074-0075
	byte		isInAnimWithCar;	//	0075-0076
	_pad(_unk2, 0x22);
	VEHICLE*	playersCar;			//	0098-009C
	VEHICLE*	carLeavingOrEntering;//	009C-00A0
	_pad(_unk3, 0x144);
	bool		isDucking;			//	01E4-01E5
	bool		isAiming;			//	01E5-01E6
	_pad(_unk4, 0x16);
	bool		isReloading;		//	01FC-01FD
	_pad(_unk5, 0x11B);
	FRAME*		frm_neck;			//	0318-031C
	_pad(_unk6, 0x184);
	INVENTARY	inventary;			//	04A0-0520
	_pad(_unk7, 0xD4);
	float		inCarRotation;		//	05F4-05F8
	_pad(_unk8, 0x4C);
	float		health;				//	0644-0648
} PED;

class CGame
{
private:
	bool bLockControls;
	bool IsStopped;
	bool ShouldStop;
	DWORD gameStart;
	byte MusicState;


public:
	CGame();
	~CGame();

	bool ShouldKill;
	bool isControlLocked();
	void SetLockControls(bool b);
	//void UpdateControls();

	void Tick();
	void StopAndWaitForGame();
	void ContinueGame();

	void UpdateCars();
	void UpdatePeds();

	static void ChangeSkin(DWORD, int);
	// vehicles
	static DWORD CreateCar(int skinID, Vector3D position, Vector3D rotation);
	static DWORD CreateCar(int skinID);
	//static DWORD CreateCar(int);

	static void ToggleVehicleEngine(DWORD vehicle, BYTE state);
	static void ToggleVehicleRoof(DWORD, BYTE);
	static void SetCarPosition(DWORD, Vector3D);
	static void SetCarRotation(DWORD, Vector3D);
	static void CarUpdate(DWORD, Vector3D, Vector3D);
	static void	CarRepair(DWORD);
	static void CarLock(DWORD, BYTE);
	static void SetPlayerPosition(DWORD PED, Vector3D position);
	static void GivePlayerToCarFast(DWORD, int, int);
	static void GivePlayerToCar(DWORD, int, int);
	static void KickPlayerFromCar(DWORD, int);
	static void KickPlayerFromCarFast(DWORD);
	static void CarJack(DWORD, DWORD, int);
	static void ExplodeCar(DWORD);
	static void LockCarDoor(DWORD, int, bool);
	// else
	static byte IsTabMapEnabled();
	static void PlayGameSound(char[]);
	static void ToggleCityMusic(byte);
	static DWORD CreatePED();
	static void DeletePed(DWORD);
	static void DeleteCar(DWORD);
	static void PlayAnim(DWORD, int);
	static void PlayAnimString(DWORD, char*);
	// Hooks
	static void Start();
	static bool GameSpawnManager();
	static void	PreRespawn();
	static void	Respawn();
	static void AfterRespawn();

	static void ThrowAwayWeapon();
	static void TakeWeapon();
	static void ChangeWeapon();
	static void HideWeapon();
	static void OnDeath();
	static void OnSuicide();
	static void OnCarShot();
	static void OnCollision();

	static void PlayerEnteredVehicle();
	static void PlayerExitVehicle();
	static void PlayerExitVehicleFinish();
	// Weapons
	static void AddWeapon(DWORD, DWORD, DWORD, DWORD, DWORD);
	static void DeleteWeapon(DWORD, DWORD);
	static void DeleteAllWeapons(DWORD ped);
	static void SwitchWeapon(DWORD, DWORD);
	static void Shoot(DWORD, Vector3D);
	static void ThrowGranade(DWORD, Vector3D);

	// Human
	static _PED*	GetLocalPED();
	static void KillPed(DWORD);
	static void KillPedEx(DWORD ped, DWORD reason, DWORD part);
	static void FixAfterDeath(DWORD ped);
	static void SetOn(DWORD, bool);
	Vector3D	GetPEDNeckPosition(PED* ped);

	// frames
	static void SetFrameModel(DWORD, char*);
	static void SetFramePos(DWORD, float, float, float);

	static DWORD PlaySoundAtFrame(DWORD, char*, float, float);
	static void  StopSound(DWORD);

	static DWORD CreateEmptyFrame();
	static DWORD FindFrame(char*);
	static DWORD FindActor(char*);

	static char* GetFrameName(FRAME* frame);

	//collisions
	static void CreateBoxCollision(DWORD);
	static void  RemoveBoxCollision(DWORD);

	//particles
	static DWORD CreateParticle(DWORD, int);
	static void RemoveParticle(DWORD);


	// camera
	static void SetCameraPos(Vector3D,float,float,float,float);
	static void CameraUnlock();
	static void	Camera_lock(DWORD);
	static void CameraSetSwing(byte, float);
	static void CameraSetFov(float);
	static float CameraGetFov();

	static void	CameraSetFocusOnObject(DWORD);
	static void CameraSetDefaultFocus();

	static Vector3D GetCameraPos();

	// map
	static void DisableBridges();

	byte GetMusicState();
	void SetMusicState(byte);

	static void ChangeMap(char map[],char*);
	static void ReloadMap();
	static void TrafficSetup();			// needed for disabling pedestrians
	static void SetTrafficVisible(bool);
	static void PoliceManager();
	static void Set8SlotInv(DWORD);

	static void TimerOn(int, int, int, int);
	static void TimerOff();

	static void  TimerSetInterval(float);
	static float TimerGetInterval();
	static void  WeatherSetParam(char*, int);
	static void  SetObjective(char*);
	static void	 ClearObjective();
	static void	 ConsoleAddText(char*, DWORD);

	void SetDoorState(char* door, bool state);
	void SetDoorStateFacing(char* door, bool shouldClose,bool facing);
	char*	GetActualMapName();
	char mapName[200];

	float loadingStatus;

	// objects
	static DWORD CreateFrame(char*);
	static void SetFrameRot(DWORD, float, float, float, float);
	static void SetFrameScale(DWORD, float, float, float);

	void PickupsTick();

	float pickupsAngle;
	
	// Returns
	// 384 - 1.0
	// 395 - 1.02
	// 0 - UNDETECTED
	static int GetGameVersion();
};

#endif