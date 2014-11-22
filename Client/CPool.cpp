#include "CPool.h"
#include "CCore.h"

extern CCore *g_CCore; 
template <class object> CPool::CPool()
{
	this->pool = new object*[100];
	//for (int i = 0; i < MAX_VEHICLES; i++)
	//	pool[i] = NULL;
	//pedpool = new CPed*[100];
}
template <class object> CPool::~CPool()
{
	delete[] & pool;
}
void CVehiclePool::New(int index, int skinID, bool isSpawned)
{
	if (vehiclepool[index] == 0)
	{
		CVehicle* pointer = new CVehicle;
		if (pointer != NULL)
		{
			vehiclepool[index] = pointer;
			pointer->SetSkinId(skinID);
			if (isSpawned)
				g_CCore->GetEngineStack()->AddMessage(ES_CREATECAR, index);
		}
	}
	else
	{
		CVehicle *veh = vehiclepool[index];
		veh->SetSkinId(skinID);
		if (veh->GetEntity() == 0)
		{
			if (isSpawned)
				g_CCore->GetEngineStack()->AddMessage(ES_CREATECAR, index);
		}
		else
		{
			g_CCore->GetChat()->AddMessage("Allready done");
		}
		veh->SetActive(1);
	}
}

CVehicle* CVehiclePool::Return(int slot)
{
	return vehiclepool[slot];
}

int CVehiclePool::GetVehicleIdByBase(DWORD base)
{
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		CVehicle* veh = this->Return(i);
		if (veh == NULL) continue;
		if (veh->GetEntity() == base) return i;
	}
	return -1;
}

void CVehiclePool::Delete(int index)
{
	delete vehiclepool[index];
	vehiclepool[index] = NULL;
}
