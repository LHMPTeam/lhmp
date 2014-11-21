#include "CPickupPool.h"
#include "CCore.h"

extern CCore *g_CCore;
CPickupPool::CPickupPool()
{
	for (int i = 0; i < MAX_PICKUPS; i++)
		pool[i] = NULL;
	//pedpool = new CPed*[100];
}
CPickupPool::~CPickupPool()
{
	delete[] & pool;
}
CPickup* CPickupPool::New(int ID, char* model, Vector3D pos, float size,bool isVisible)
{
	pool[ID] = new CPickup(model,pos,size,isVisible);
	return pool[ID];
}

CPickup* CPickupPool::Return(int slot)
{
	return pool[slot];
}

int CPickupPool::GetObjectIdByBase(DWORD base)
{
}

void CPickupPool::Delete(int index)
{
	delete pool[index];
	pool[index] = NULL;
}
