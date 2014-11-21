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

	bool isControlLocked();
	void SetLockControls(bool b);
	void UpdateControls();

	void Tick();
	void StopAndWaitForGame();
	void ContinueGame();

	void UpdateCars();
	void UpdatePeds();

	static void ChangeSkin(DWORD,int);
	// vehicles
	static DWORD CreateCar(int);
	static void ToggleVehicleRoof(DWORD, BYTE);
	static void SetCarPosition(DWORD, Vector3D);
	static void SetCarRotation(DWORD, Vector3D);
	static void CarUpdate(DWORD, Vector3D,Vector3D);
	static void	CarRepair(DWORD);
	static void CarLock(DWORD, BYTE);
	static void GivePlayerToCarFast(DWORD, int, int);
	static void GivePlayerToCar(DWORD, int, int);
	static void KickPlayerFromCar(DWORD, int);
	static void KickPlayerFromCarFast(DWORD);
	static void CarJack(DWORD, DWORD, int);
	static void ExplodeCar(DWORD);
	static void LockCarDoor(DWORD, int, bool);
	// else
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
	static void AddWeapon(DWORD,DWORD,DWORD,DWORD,DWORD);
	static void DeleteWeapon(DWORD, DWORD);
	static void SwitchWeapon(DWORD,DWORD);
	static void Shoot(DWORD,Vector3D);
	static void ThrowGranade(DWORD,Vector3D);

	// Human
	static void KillPed(DWORD);
	static void SetOn(DWORD,bool);

	// frames
	static void SetFrameModel(DWORD, char*);
	static void SetFramePos(DWORD,float,float,float);

	static DWORD CreateSoundFrame(DWORD, char*, float, float);
	static void  RemoveSoundFrame(DWORD);

	static DWORD CreateEmptyFrame();
	static DWORD FindFrame(char*);
	static DWORD FindActor(char*);

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
	char*	GetActualMapName();
	char mapName[200];

	float loadingStatus;

	// objects
	static DWORD CreateFrame(char*);
	static void SetFrameRot(DWORD, float, float, float, float);
	static void SetFrameScale(DWORD, float, float, float);

	void PickupsTick();

	float pickupsAngle;
};

#endif