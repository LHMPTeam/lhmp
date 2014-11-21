#include "CVehiclePool.h"

CVehiclePool::CVehiclePool()
{
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		vehiclepool[i] = NULL;
	}
}
CVehiclePool::~CVehiclePool()
{
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		if (vehiclepool[i] != NULL)
		{
			delete vehiclepool[i];
		}
	}
}

int CVehiclePool::New()
{
	//playerpool[index] = new CPlayer[1];
	for (int i = 0; i < MAX_VEHICLES; i++)
	if (vehiclepool[i] == NULL)
	{
		CVehicle* pointer = new CVehicle;
		vehiclepool[i] = pointer;
		return i;
	}
	return -1;
}

CVehicle* CVehiclePool::Return(int index)
{
	if (index >= 0 && index < MAX_VEHICLES)
		return vehiclepool[index];
	else
		return NULL;
}

void CVehiclePool::Delete(int index)
{
	delete vehiclepool[index];
	vehiclepool[index] = NULL;
}

int	CVehiclePool::ReturnId(CVehicle* p)
{
	for (int i = 0; i < MAX_VEHICLES; i++)
	{
		if (vehiclepool[i] == p)
			return i;
	}
	return -1;
}