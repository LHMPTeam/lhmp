#ifndef CPLAYER_H
#define CPLAYER_H
#include "../shared/structures.h"
#include "CEntity.h"
class CPlayer: public CEntity
{
private:
	char			nickname[255];
	
	bool			isDucking;
	bool			isAim;

	// Weapons
	int				currentWep;
	SWeapon			weapon[8];

	float			carAim;
	float			fov;
	int				money;

	bool			isLocked;

public:
	CPlayer();
	~CPlayer();

	void SetNickname(char[]);
	void SetNickname(const char*);
	void SetIsDucking(bool);
	void SetIsAim(bool);
	void SetIsActive(bool);
	void SetCurrentWeapon(int);
	void SetCarAim(float);
	void SetMoney(int);
	void SetFOV(float);

	// Weapon actions
	void AddWeapon(int,int,int);
	void DeleteWeapon(int);
	void SwitchWeapon(int);
	void OnShoot();
	void ResetWeapons();

	char*	GetNickname();
	bool	IsDucking();
	bool	IsAim();
	bool    IsActive();
	int		GetCurrentWeapon();
	SWeapon* GetWeapon(int);

	int				InCar;

	float	GetCarAim();
	int		GetMoney();
	float	GetFOV();

	void OnThrowGranade();

	bool	IsLocked();
	void	SetLocked(bool);

	//------ Network functions
	void	OnSync(SYNC::ON_FOOT_SYNC,RakNet::TimeMS);
	void	OnCarUpdate(SYNC::IN_CAR);
	void	OnChangeSkin(int);
	void	OnRespawn();
	void	OnAddWeapon(int,int,int,bool = false);
	void	OnDeleteWeapon(int, bool = false);
	void	OnSwitchWeapon(int, bool = false);
	void	OnPlayerShoot(float, float, float, bool = false);
	void	OnPlayerThrowGranade(Vector3D, bool = false);
};

#endif