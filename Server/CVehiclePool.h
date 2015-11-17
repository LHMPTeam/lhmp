/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose pool of vehicles
@author Romop5
******************************************************************************/

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