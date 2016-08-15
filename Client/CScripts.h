// (C) LHMP Team 2013-2016; Licensed under Apache 2; See LICENSE;;

// regular client-side Squirrel script

#include "CSquirrel.h"
#include <iostream>
#include <vector>

// Class for each Squirrel script
class CScript
{
public:
	CScript(char* name);
	char* GetScriptName();
	HSQUIRRELVM GetVM()
	{
		return this->pVirtualMachine;
	}
private:
	char scriptname[255];
	HSQUIRRELVM pVirtualMachine;

};

class CScripts
{
public:
	void AddScript(char* name);
	void Reset();
private:
	std::vector <CScript> scriptsPool;
	
};