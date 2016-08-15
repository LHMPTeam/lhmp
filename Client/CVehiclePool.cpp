// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#include "CVehiclePool.h"
#include "CCore.h"

extern CCore *g_CCore;
CVehiclePool::CVehiclePool()
{
	for (int i = 0; i < MAX_VEHICLES; i++)
		vehiclepool[i] = NULL;
	//pedpool = new CPed*[100];
}
CVehiclePool::~CVehiclePool()
{
	delete[] & vehiclepool;
}

void	CVehiclePool::New(int vehicleID, int skinID, Vector3D position, Vector3D rotation,bool isSpawned)
{
	if (vehiclepool[vehicleID] == 0)
	{
		CVehicle* pointer = new CVehicle;
		if (pointer != NULL)
		{
			vehiclepool[vehicleID] = pointer;
			pointer->SetSkinId(skinID);
			pointer->SetPosition(position);
			pointer->SetPosition(rotation);
			if (isSpawned)
				g_CCore->GetEngineStack()->AddMessage(ES_CREATECAR, vehicleID);
		}
	}
	else
	{
		CVehicle *veh = vehiclepool[vehicleID];
		veh->SetSkinId(skinID);
		veh->SetPosition(position);
		veh->SetPosition(rotation);
		if (veh->GetEntity() == 0)
		{
			if (isSpawned)
				g_CCore->GetEngineStack()->AddMessage(ES_CREATECAR, vehicleID);
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
