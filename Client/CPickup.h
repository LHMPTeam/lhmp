/**
Lost Heaven Multiplayer

Purpose: pickup (pickable 3D model) base class

@author Romop5
@version 1.0 1/9/14
@todo clean code
*/

#ifndef CPICKUP_H
#define CPICKUP_H
#include "../shared/structures.h"
class CPickup
{
private:
	Vector3D	pPosition;
	float		pSize;
	char		pModel[200];
	bool		pIsVisible;

	DWORD		pEntity;

public:
	CPickup(char* model, Vector3D,float,bool);
	void SetPosition(Vector3D);
	Vector3D GetPosition();

	void SetSize(float);
	float GetSize();

	char*	GetModel();
	void	SetModel(char* model);

	bool	IsVisible();
	void	SetVisible(bool f);

	DWORD	GetEntity();
	void	SetEntity(DWORD);
};
#endif