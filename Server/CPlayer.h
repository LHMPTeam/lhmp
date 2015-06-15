#ifndef CPLAYER_H
#define CPLAYER_H
#include "../shared/structures.h"
#include "CEntity.h"
class CPlayer: public CEntity
{
public:
	CPlayer();
	~CPlayer();

	void SetFloatRotation(float);
	float GetFloatRotation();

	void	SetNickname(char[]);
	void	SetNickname(const char*);
	char*	GetNickname();
	void	SetIsDucking(bool);
	bool	IsDucking();
	void	SetIsAim(bool);
	bool	IsAim();
	void	SetIsActive(bool);
	bool    IsActive();
	void	SetCurrentWeapon(int);
	int		GetCurrentWeapon();
	void	SetCarAim(float);
	float	GetCarAim();
	void	SetIsCarAnim(bool);
	bool	IsCarAnim();
	void	SetMoney(int);
	int		GetMoney();
	void	SetFOV(float);
	float	GetFOV();

	void	SetCurrentCar(int);
	int		GetCurrentCar();

	void	SetNickColor(unsigned int);
	unsigned int GetNickColor();

	// Weapon actions
	void AddWeapon(int,int,int);
	void DeleteWeapon(int);
	void SwitchWeapon(int);
	void OnShoot();
	void ResetWeapons();
	void OnThrowGranade();

	SWeapon* GetWeapon(int);

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

	void	net_ChangeNickColor(unsigned int color);

private:
	char			nickname[255];

	bool			isDucking;
	bool			isAim;
	bool			isCarAnim;

	// ID of car that is used by player
	int				playerInCarID;

	// Weapons
	int				currentWep;
	SWeapon			weapon[8];

	float			carAim;
	float			fov;
	int				money;

	bool			isLocked;

	float			floatRot;

	unsigned int	nickColor;
};

#endif