#include "CDoorPool.h"
#include <string.h>
#define NULL 0
CDoorPool::CDoorPool()
{
	start = 0;
	end = 0;
}
void CDoorPool::Push(char* name, bool state,bool facing)
{
	if (start == NULL)
	{
		this->Add(name, state,facing);
	} else
	{
		sDoor* item = start;
		while (item != NULL)
		{
			// names are equal => we found right element
			if (strcmp(item->name, name) == 0)
			{
				item->state = state;
				item->facing = facing;
				return;
			}
			else {
				item = item->nextDoor;
			}
		}
		// if there is no element with name, create a new one
		this->Add(name, state,facing);
	}
}

void CDoorPool::Add(char* name, bool state, bool facing)
{
	sDoor* door = new sDoor(name,state,facing);
	if (end == NULL && start == NULL)
	{
		end = door;
		start = door;
	}
	else{
		end->nextDoor = door;
		end = door;
	}
}

sDoor* CDoorPool::GetStart()
{
	return this->start;
}