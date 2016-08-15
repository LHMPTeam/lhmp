// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

#ifndef CDOORPOOL_H
#define CDOORPOOL_H

#include <stdio.h>
struct sDoor
{
	char name[200];
	bool state;
	bool facing;
	sDoor* nextDoor;
	sDoor(char* nm, bool st,bool fc)
	{
		state = st;
		sprintf(name, nm);
		nextDoor = 0;
		facing = fc;
	}
};
class CDoorPool
{
private:
	sDoor* start, *end;
	void Add(char*, bool state, bool facing);
public:
	CDoorPool();

	// Push new element (edits if element already exists)
	void Push(char*, bool state,bool facing);
	
	// Returns the handle of the first pool element
	sDoor* GetStart();

	// this will completly delete all members of pool -> default, clean pool
	void Reset();
};

#endif