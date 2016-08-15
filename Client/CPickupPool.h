// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __PICKUP_H
#define __PICKUP_H
#include "CPickup.h"
#include "../shared/limits.h"

class CPickupPool
{
private:
	CPickup* pool[MAX_PICKUPS];
public:
	CPickupPool();
	~CPickupPool();
	CPickup*	New(int,char* model, Vector3D pos,float size,bool isVisible);
	CPickup*	Return(unsigned int);
	int			GetObjectIdByBase(DWORD);
	void		Delete(int);
};
#endif