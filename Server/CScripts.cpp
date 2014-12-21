#include "CScripts.h"

CScripts::CScripts()
{
	// reset all script slots
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		m_pScripts[i] = NULL;
	}
}

CScripts::~CScripts()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i] != NULL) delete m_pScripts[i];
		m_pScripts[i] = NULL;
	}
}

bool CScripts::LoadScript(const char * szScriptName)
{
	// make sure a script with the same name isn't already loaded
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (!m_pScripts[i]) continue;
		if (!strcmp(m_pScripts[i]->GetScriptName(), szScriptName)) {
			// a script with the same name already exists
			return false;
		}
	}

	// find a free script slot
	int iSlot = -1;

	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (!m_pScripts[i]) {
			// found a free slot
			iSlot = i;
			break;
		}
	}

	if (iSlot == -1) {
		// no free script slot found
		return false;
	}

	if (m_pScripts[iSlot] == NULL)
	{
		m_pScripts[iSlot] = new CScript(szScriptName);
	}
	// script loaded successfully
	return true;
}

bool CScripts::UnloadScript(const char * szScriptName)
{
	// find the script slot
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (!strcmp(m_pScripts[i]->GetScriptName(), szScriptName)) {
			// found the script slot, unload the script

			delete m_pScripts[i];

			m_pScripts[i] = NULL;
			// script unloaded successfully
			return true;
		}
	}

	// script not found
	return false;
}

void CScripts::UnloadAll()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i] != NULL)
		{
			delete m_pScripts[i];

			m_pScripts[i] = NULL;
		}
	}
}

void CScripts::Call(const char * szFunc, int iArgCount, SQObject * pArguments)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, szFunc, -1);

			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				if (pArguments != NULL)
				{
					for (int j = 0; j < iArgCount; ++j)
						sq_pushobject(pVM, pArguments[j]);
					//pVM->Push(pArguments[j]);
				}
				sq_call(pVM, iArgCount + 1, true, true);

			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerInit()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerInit", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onServerExit()
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerExit", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Call the function
				sq_call(pVM, 1, true, true);
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

bool CScripts::onPlayerText(int playerId, const char *text)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerText", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, true, true)))
				{
					SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
	return ret;
}
void CScripts::onPlayerCommand(int playerId, const char *text,const char* params)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerCommand", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerId);

				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 4, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerConnect(int playerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerConnect", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);

			/*	// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);
				*/
				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onServerTick(int ticks)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerTick", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, ticks);

				/*	// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);
				*/
				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onServerTickSecond(int ticks)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onServerTickSecond", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, ticks);

				/*	// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);
				*/
				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onPlayerDisconnect(int playerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerDisconnect", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);
				/*
				// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onPlayerEnterVehicle(int playerID, int vehID, int seatID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerEnterVehicle", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);
				sq_pushinteger(pVM, vehID);
				sq_pushinteger(pVM, seatID);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 4, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}
void CScripts::onPlayerExitVehicle(int playerID, int vehID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerExitVehicle", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);
				sq_pushinteger(pVM, vehID);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerSpawn(int playerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerSpawn", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 2, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerIsKilled(int playerID,int killerID)
{
	bool ret = true;
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerIsKilled", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack
				sq_pushinteger(pVM, playerID);

				sq_pushinteger(pVM, killerID);

				/*// Push the text onto the stack
				sq_pushstring(pVM, text, -1);

				sq_pushstring(pVM, params, -2);*/

				// Call the function
				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
					/*SQBool result;
					sq_getbool(pVM, sq_gettop(pVM), &result);
					if (result == false) ret = false;*/
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerKeyPressed(int ID,int key)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerKeyPressed", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);
				sq_pushinteger(pVM, key);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerShoot(int ID, int weaponID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerShoot", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);
				sq_pushinteger(pVM, weaponID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPlayerHit(int ID,int attackerID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPlayerHit", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, ID);
				sq_pushinteger(pVM, attackerID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}

void CScripts::onPickupTaken(int pickupID, int playerID)
{
	for (int i = 0; i < MAX_SCRIPTS; i++) {
		if (m_pScripts[i]) {
			// get the script vm pointer
			SQVM * pVM = m_pScripts[i]->GetVM();

			// Get the stack top
			int iTop = sq_gettop(pVM);

			// Push the root table onto the stack
			sq_pushroottable(pVM);

			// Push the function name onto the stack
			sq_pushstring(pVM, "onPickupTaken", -1);

			// Get the closure for the function
			if (SQ_SUCCEEDED(sq_get(pVM, -2))) {
				// Push the root table onto the stack
				sq_pushroottable(pVM);

				// Push the player id onto the stack

				sq_pushinteger(pVM, pickupID);
				sq_pushinteger(pVM, playerID);

				// Call the function
				if (!SQ_FAILED(sq_call(pVM, 3, false, true)))
				{
				}
			}

			// Restore the stack top
			sq_settop(pVM, iTop);
		}
	}
}