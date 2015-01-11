/**
Lost Heaven Multiplayer

Purpose: handles all synced doors

@author Romop5
@version 1.0 1/9/14
*/

#ifndef CDOORPOOL_H
#define CDOORPOOL_H

#include <stdio.h>
struct sDoor
{
	char name[200];
	bool state;
	sDoor* nextDoor;
	sDoor(char* nm, bool st)
	{
		state = st;
		sprintf(name, nm);
		nextDoor = 0;
	}
};
class CDoorPool
{
private:
	sDoor* start, *end;
	void Add(char*, bool state);
public:
	CDoorPool();
	void Push(char*, bool state);
	void Pop();
	sDoor* GetStart();
};

#endif