// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

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
	// loop through the pool to find out if the same element exists
	// if not, create a new one 
	sDoor* item = start;
	while (item != NULL)
	{
		// if element with same name exists
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

// Returns the first element
sDoor* CDoorPool::GetStart()
{
	return this->start;
}

// Deletes all pool elements
void CDoorPool::Reset()
{
	sDoor* pointer = this->GetStart();
	this->start = NULL;
	while (pointer != NULL)
	{
		sDoor* current = pointer;
		pointer = pointer->nextDoor;
		delete current;
	}
}