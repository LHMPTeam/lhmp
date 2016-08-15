// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef __VEHICLEPOOL_H
#define	__VEHICLEPOOL_H
#include "../shared/limits.h"
#include "CVehicle.h"

class CVehiclePool
{
private:
	CVehicle* vehiclepool[MAX_VEHICLES];
public:
	CVehiclePool();
	~CVehiclePool();

	void	New(int vehicleID, int skinID, Vector3D position, Vector3D rotation, bool isSpawned = true);
	CVehicle*	Return(int);
	int		GetVehicleIdByBase(DWORD);
	void	Delete(int);
};
#endif