#ifndef PED_H
#define	PED_H
#include "CEntity.h"
#include "../shared/structures.h"
class CPed: public CEntity
{
private:
	bool		isOnFoot;
	//bool		isActive;
	//Vector3D	playerPos;
	char		sName[80];
	//float		fHealth;
	//float		rotation1;
	//float		rotation2;
	//float		rotation3;
	//byte		state;
	byte		isSpawned;
	byte		isDucking;
	byte		isAiming;
	//int			skinId;
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
	//DWORD		pedBase;
	//DWORD		frame;
	CPed();
	//~CPed();
	void		UpdateGameObject();
	//void		SetEntity(DWORD);
	//void		SetPosition(Vector3D vect);
	void		SetName(char[]);
	//void		SetHealth(float);
	//void		SetRotation(float,float,float);
	//void		SetState(byte);
	void		SetDucking(byte);
	void		SetAiming(byte);
	//void		SetSkinId(int);
	void		SetPing(int);
	void		SetActive(bool);

	bool		IsActive();
	int			GetPing();
	char*		GetName();
	//float		GetHealth();
	//void		SetRotation(float,float);
	//byte		GetState();
	//Vector3D	GetPosition();
	//bool		HasBase();
	
	//int		GetSkin();
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