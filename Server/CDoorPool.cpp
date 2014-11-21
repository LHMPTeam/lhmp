#include "CDoorPool.h"
#include <string.h>
#define NULL 0
CDoorPool::CDoorPool()
{
	start = 0;
	end = 0;
}
void CDoorPool::Push(char* name, bool state)
{
	if (start == NULL)
	{
		this->Add(name, state);
	} else
	{
		sDoor* item = start;
		while (item != NULL)
		{
			if (strcmp(item->name, name) == 0)
			{
				item->state = state;
				return;
			}
			else {
				item = item->nextDoor;
			}
		}
		this->Add(name, state);
	}
}

void CDoorPool::Add(char* name, bool state)
{
	sDoor* door = new sDoor(name,state);
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