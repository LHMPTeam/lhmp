/******************************************************************************
Lost Heaven Multiplayer project
See LICENSE in the top level directory

@purpose pickup pool
@author Romop5
******************************************************************************/

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
	// Tests player for picking up any of pickups
	void		TestPlayer(int ID);
	void		Tick();

	// Deletes all pickups & returns to default state
	void		Reset();
};