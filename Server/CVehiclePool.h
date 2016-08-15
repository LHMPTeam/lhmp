// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "../shared/limits.h"
#include "CVehicle.h"
class CVehiclePool
{
private:
	CVehicle*	vehiclepool[MAX_VEHICLES];
public:
	CVehiclePool();
	~CVehiclePool();
	int			New();
	void		Delete(int);
	CVehicle*	Return(int);
	int			ReturnId(CVehicle*);
	//void		DeleteVehicle(int);
};