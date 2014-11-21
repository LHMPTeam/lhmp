/**
	Lost Heaven Multiplayer

	Purpose: vehicle pool (array)

	@author Romop5
	@version 1.0 1/9/14
	@todo clean code
*/

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
	void	New(int,int,bool isSpawned = true);
	CVehicle*	Return(int);
	int		GetVehicleIdByBase(DWORD);
	void	Delete(int);
};
#endif