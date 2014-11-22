#include "../shared/limits.h"
#include "CPickup.h"
class CPickupPool
{
private:
	CPickup*	pool[MAX_PICKUPS];
public:
	CPickupPool();
	~CPickupPool();
	int			New();
	int			New(char* model, int interval,float x, float y, float z, float size);
	void		Delete(int);
	CPickup*	Return(int);
	int			ReturnId(CPickup*);
	void		TestPlayer(int ID);
	void		Tick();
	//void		DeleteVehicle(int);
};