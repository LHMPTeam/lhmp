#ifndef VEHICLEPOOL_H
#define	VEHICLEPOOL_H
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