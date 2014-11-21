//-----------------------------------------------------
//
// VC:Players Multiplayer Modification For GTA:VC
// Copyright 2010 GTA:Online team
//
// File Authors: adamix
//
// License: See LICENSE in root directory
//
//-----------------------------------------------------

//#include "main.h"
//#include "CScripts.h"

#include <stdarg.h> 
#include <squirrel.h> 
#include <sqstdio.h> 
#include <sqstdaux.h> 

void RegisterFunction(HSQUIRRELVM pVM, char * szFunc, SQFUNCTION func, int params, const char * szTemplate);
void RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, bool bValue);
void RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, int iValue);
void RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, float fValue);
void RegisterVariable(HSQUIRRELVM pVM, const char * szVarName, const char * szValue);