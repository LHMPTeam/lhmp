#include "CPickupPool.h"

CPickupPool::CPickupPool()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		pool[i] = NULL;
	}
}
CPickupPool::~CPickupPool()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (pool[i] != NULL)
		{
			delete pool[i];
		}
	}
}

int CPickupPool::New()
{
	//playerpool[index] = new CPlayer[1];
	for (int i = 0; i < MAX_PICKUPS; i++)
	if (pool[i] == NULL)
	{
		CPickup* pointer = new CPickup;
		pool[i] = pointer;
		return i;
	}
	return -1;
}

int CPickupPool::New(char* model, int interval, float x, float y, float z, float size)
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	if (pool[i] == NULL)
	{
		CPickup* pointer = new CPickup(i,model,interval,x,y,z,size);
		pool[i] = pointer;
		return i;
	}
	return -1;
}

CPickup* CPickupPool::Return(int index)
{
	if (index >= 0 && index < MAX_PICKUPS)
		return pool[index];
	else
		return NULL;
}

void CPickupPool::Delete(int index)
{
	delete pool[index];
	pool[index] = NULL;
}

int	CPickupPool::ReturnId(CPickup* p)
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (pool[i] == p)
			return i;
	}
	return -1;
}

void	CPickupPool::TestPlayer(int ID)
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (pool[i] != NULL)
		{
			pool[i]->IsTakingPickup(ID);
		}
	}
}


void	CPickupPool::Tick()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (pool[i] != NULL)
		{
			pool[i]->Tick();
		}
	}
}


void	CPickupPool::Reset()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
	{
		if (pool[i])
		{
			delete pool[i];
			pool[i] = NULL;
		}
	}
}