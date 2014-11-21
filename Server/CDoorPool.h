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