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

CPickup* CPickupPool::Return(unsigned int slot)
{
	if (slot >= MAX_PICKUPS)
	{
		// generate error
		char buff[250];
		sprintf(buff, "[Err] SLOT overlaps MAX_PICKUPS - caller [%p] %s:%d", _ReturnAddress(),__FILE__, __LINE__);
		g_CCore->GetLog()->AddLog(buff);
		return NULL;
	}
	return pool[slot];
}

int CPickupPool::GetObjectIdByBase(DWORD base)
{
	return NULL;
}

void CPickupPool::Delete(int index)
{
	delete pool[index];
	pool[index] = NULL;
}
