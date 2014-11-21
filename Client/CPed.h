/**
	Lost Heaven Multiplayer

	Purpose: pedestrian (remote player) class

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code
*/

#ifndef __PED_H
#define	__PED_H
#include "CEntity.h"
#include "../shared/structures.h"
class CPed: public CEntity
{
private:
	bool		isOnFoot;
	char		sName[80];
	byte		isSpawned;
	byte		isDucking;
	byte		isAiming;
	int			animId;

	int			ping;
	// interpolation
	Vector3D			actual,previous;
	// weapons
	int				currentWep;
	SWeapon			weapon[8];


public:
	void		SetIsOnFoot(bool);
	bool		IsOnFoot();
	CPed();
	void		UpdateGameObject();
	void		SetName(char[]);
	void		SetDucking(byte);
	void		SetAiming(byte);
	void		SetPing(int);
	void		SetActive(bool);

	bool		IsActive();
	int			GetPing();
	char*		GetName();
	byte		IsDucking();
	
	void		SetAnim(int);
	int			GetAnim();
	void		SetUpInterpolation();
	void		Interpolate();

	virtual Vector3D GetPosition();

	// Weapon actions
	SWeapon* GetWeapon(int);
	int	 GetCurrentWeapon();
	void SetCurrentWeapon(int);
	void AddWeapon(int,int,int);
	void DeleteWeapon(int);
	void SwitchWeapon(int);
	void OnShoot();
	void OnThrowGranade();
	void ClearWeapons();

	int			InCar;
};
#endif