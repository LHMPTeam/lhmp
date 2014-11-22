#ifndef CPICKUP_H
#define CPICKUP_H
#include "../shared/structures.h"
class CPickup
{
private:
	int			ID;
	Vector3D	pPos;
	float		pSize;
	char		pModel[250];
	int			interval;
	bool		pIsVisible;
	DWORD		respawnTime;
public:
	CPickup();
	~CPickup();
	CPickup(int ID, char* model, int interval, float x, float y, float z, float size);

	int		GetID();
	char*	GetModel();
	void	SetModel(char*);

	void	SetPosition(Vector3D);
	Vector3D GetPosition();

	void	SetSize(float);
	float	GetSize();


	bool	IsVisible();
	void	SetVisible(bool);

	bool	IsTakingPickup(int ID);

	// network
	void SendIt(int IDorothers);
	void SendVisible(int IDorOthers,bool visible);
	void SendDelete(int IDorothers);

	void	Tick();
};

#endif