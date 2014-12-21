#include "sq_funcdef.h"
#include "CCore.h"

#include <fstream>
#include <sstream>
#include <ios>
extern CCore* g_CCore;

SQInteger sq_dopice(SQVM *squirrelVM)
{
	//std::cout << "picewtf" << std::endl;
	//sq_pushroottable(squirrelVM);                              // +1

	// The name the array will have in the root table.
	//sq_pushstring(squirrelVM, "sampleArray", -1);              // +1

	// Push the array on the stack.
	sq_newarray(squirrelVM, 0);                                // +1

	// Push an integer on the stack and append it to the array.
	sq_pushinteger(squirrelVM, 50);                            // +1
	sq_arrayappend(squirrelVM, -2);                            // -1

	// Push a string on the stack and append it to the array.
	sq_pushstring(squirrelVM, _SC("This is a string."), -1);   // +1
	sq_arrayappend(squirrelVM, -2);                            // -1

	// Push another array on the stack.
	//sq_newarray(squirrelVM, 0);                                // +1

	/*// Append intArray's values to the new array.
	for (int i = 0; i < intArrayLength; i++) {
		sq_pushinteger(squirrelVM, intArray[i]);               // +1
		sq_arrayappend(squirrelVM, -2);                        // -1
	}*/

	// Append the new array to the previous array.
	//sq_arrayappend(squirrelVM, -2);                            // -1

	// Set the array in the root table.
	//sq_newslot(squirrelVM, -3, SQFalse);                       // -2

	// Pop the root table.
	//sq_poptop(squirrelVM);                                     // -1
	return 1;
}

SQInteger sq_playerSetWeatherParam(SQVM *vm)
{
	SQInteger ID, param_val;
	const SQChar* param;
	sq_getinteger(vm, -3, &ID);
	sq_getstring(vm, -2, &param);
	sq_getinteger(vm, -1, &param_val);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_WEATHER_PARAM);
		bsOut.Write(param);
		bsOut.Write(param_val);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
		g_CCore->GetLog()->AddDebug("playerSetWeatherParam - %d %s %d",ID,param,param_val);
	}
	return 1;
}

SQInteger sq_playerSetCameraSwing(SQVM *vm)
{
	SQInteger ID, enable;
	SQFloat force;

	sq_getinteger(vm, -3, &ID);
	sq_getinteger(vm, -2, &enable);
	sq_getfloat(vm, -1, &force);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_CAMERA_SWING);
		bsOut.Write(enable);
		bsOut.Write(force);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
		g_CCore->GetLog()->AddDebug("playerSetWeatherParam - %d %d %f", ID, enable,force);
	}
	return 1;
}

SQInteger sq_playerSetCameraFov(SQVM *vm)
{
	SQInteger ID;
	SQFloat FOV;

	sq_getinteger(vm, -2, &ID);
	sq_getfloat(vm, -1, &FOV);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);

	if (player != NULL)
	{
		player->SetFOV(FOV);

		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_CAMERA_FOV);
		bsOut.Write(FOV);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
		g_CCore->GetLog()->AddDebug("playerSetCameraFov - %d %f", ID,FOV);
	}
	return 1;
}

SQInteger sq_playerGetCameraFov(SQVM *vm)
{
	SQInteger ID;

	sq_getinteger(vm, -1, &ID);
	g_CCore->GetLog()->AddDebug("playerGetCameraFov - ID %d", ID);
	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		sq_pushfloat(vm, player->GetFOV());
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}


SQInteger sq_playerTimerOn(SQVM *vm)
{
	SQInteger ID, remain, seconds, minutes, hours;

	sq_getinteger(vm, -5, &ID);
	sq_getinteger(vm, -4, &remain);
	sq_getinteger(vm, -3, &seconds);
	sq_getinteger(vm, -2, &minutes);
	sq_getinteger(vm, -1, &hours);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_TIMER_ON);
		bsOut.Write(remain);
		bsOut.Write(seconds);
		bsOut.Write(minutes);
		bsOut.Write(hours);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
	}
	return 1;
}

SQInteger sq_playerTimerOff(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_TIMER_OFF);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
	}
	return 1;
}

SQInteger sq_playerSetTimerInterval(SQVM *vm)
{
	SQInteger ID;
	SQFloat interval;

	sq_getinteger(vm, -2, &ID);
	sq_getfloat(vm, -1, &interval);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_TIMER_INTERVAL);
		bsOut.Write(interval);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
	}

	return 1;
}

SQInteger sq_playerGetTimerInterval(SQVM *vm)
{
	return 1;
}


SQInteger sq_playerSetObjective(SQVM *vm)
{
	SQInteger ID;
	const SQChar* objective;

	sq_getinteger(vm, -2, &ID);
	sq_getstring(vm, -1, &objective);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_OBJECTIVE);
		bsOut.Write(objective);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
	}
	return 1;
}

SQInteger sq_playerClearObjective(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_CLEAR_OBJECTIVE);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
	}
	return 1;
}

SQInteger sq_playerConsoleAddText(SQVM *vm)
{
	SQInteger ID;
	const SQChar* text;
	const SQChar* color;

	sq_getinteger(vm, -3, &ID);
	sq_getstring(vm, -2, &color);
	sq_getstring(vm, -1, &text);


	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_ADD_CONSOLE_TEXT);
		bsOut.Write(color);
		bsOut.Write(text);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
	}

	return 1;
}

SQInteger sq_playerLockControls(SQVM *vm)
{
	SQInteger ID;
	SQBool	shouldLock;
	sq_getinteger(vm, -2, &ID);
	sq_getbool(vm, -1, &shouldLock);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_LOCK_CONTROLS);
		bsOut.Write(shouldLock);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),  false);
		player->SetLocked(shouldLock == 1);
	}
	return 1;
}

SQInteger sq_playerIsLocked(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		sq_pushbool(vm, player->IsLocked());
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_sendPlayerMessage(SQVM *vm)
{
	SQInteger	ID;
	const SQChar*		pMessage;
	sq_getinteger(vm, -2, &ID);
	sq_getstring(vm, -1, &pMessage);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_CHAT_MESSAGE);
	bsOut.Write(pMessage);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
	return 1;
}
SQInteger sq_sendAllMessage(SQVM *vm)
{
	const SQChar*		pMessage;
	sq_getstring(vm, -1, &pMessage);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_CHAT_MESSAGE);
	bsOut.Write(pMessage);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	return 1;
}

SQInteger sq_playerChangeSkin(SQVM *vm)
{
	SQInteger	ID,skinID;
	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &skinID);

	if (skinID > 303)
		return 1;
	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		/*player->SetSkin(skinID);
		//	BitStream bsOut;
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_CHANGESKIN);
		bsOut.Write(ID);
		bsOut.Write(skinID);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);*/
		player->OnChangeSkin(skinID);
	}
	return 1;
}

SQInteger sq_playerToggleCityMusic(SQVM *vm)
{
	SQInteger ID, state;
	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &state);

	state = !state;

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_TOGGLE_CITY_MUSIC);
		bsOut.Write(state);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);
	}
	return 1;

}

SQInteger sq_playerGetSkinID(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		sq_pushinteger(vm, player->GetSkin());
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_playerGetHealth(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		sq_pushfloat(vm, player->GetHealth());
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_playerIsConnected(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		sq_pushbool(vm, true);
		return 1;
	}
	else sq_pushbool(vm, false);

	sq_pushnull(vm);
	return 1;
}

SQInteger sq_playerGetPosition(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		Vector3D pos = player->GetPosition();
		sq_newarray(vm, 0);
		sq_pushfloat(vm, pos.x);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.y);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.z);
		sq_arrayappend(vm, -2);
		sq_push(vm, -1);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_playerGetRotation(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		Vector3D pos = player->GetRotation();
		sq_newarray(vm, 0);
		sq_pushfloat(vm, pos.x);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.y);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.z);
		sq_arrayappend(vm, -2);
		sq_push(vm, -1);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_playerInVehicleID(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		sq_pushinteger(vm, player->InCar);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_playerSetPosition(SQVM *vm)
{
	SQInteger	ID;
	Vector3D	pos;
	sq_getinteger(vm, -4, &ID);
	sq_getfloat(vm, -3, &pos.x);
	sq_getfloat(vm, -2, &pos.y);
	sq_getfloat(vm, -1, &pos.z);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		player->SetPosition(pos);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_POSITION);
		bsOut.Write(ID);
		bsOut.Write(pos);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm,false);
	return 1;
}
SQInteger sq_playerSetHealth(SQVM *vm)
{
	SQInteger	ID;
	float		health;
	sq_getinteger(vm, -2, &ID);
	sq_getfloat(vm, -1, &health);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		player->SetHealth(health);

		if (health > 0)
		{
			BitStream bsOut;
			bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((MessageID)LHMP_PLAYER_SET_HEALTH);
			bsOut.Write(ID);
			bsOut.Write(health);
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		}
		else
		{
			g_CCore->GetScripts()->onPlayerIsKilled(ID, ID);
			BitStream bsOut;
			bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((MessageID)LHMP_PLAYER_DEATH);
			bsOut.Write(ID);
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			if (player->InCar != -1)
			{
				g_CCore->GetVehiclePool()->Return(player->InCar)->PlayerExit(ID);
			}
		}
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}
SQInteger sq_playerGetMoney(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		sq_pushinteger(vm, player->GetMoney());
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_guiToggleNametag(SQVM *vm)
{
	SQInteger	ID;
	SQBool		status;
	sq_getinteger(vm, -2, &ID);
	sq_getbool(vm, -1, &status);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_NAMETAG);
		bsOut.Write(status);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_playerSetMoney(SQVM *vm)
{
	SQInteger	ID;
	SQInteger	money;
	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &money);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_MONEY);
		bsOut.Write(money);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);

		player->SetMoney(money);

		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_playerEnableMoney(SQVM *vm)
{
	SQInteger ID;
	SQInteger enable;

	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &enable);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{

		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_ENABLE_MONEY);
		bsOut.Write(enable);

		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID),false);

		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_playerSetRotation(SQVM *vm)
{
	SQInteger	ID;
	Vector3D	rot;
	sq_getinteger(vm, -4, &ID);
	sq_getfloat(vm, -3, &rot.x);
	sq_getfloat(vm, -2, &rot.y);
	sq_getfloat(vm, -1, &rot.z);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		player->SetRotation(rot);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_ROTATION);
		bsOut.Write(ID);
		bsOut.Write(rot);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}
SQInteger sq_playerGetIP(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);
	SystemAddress sa = g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID);
	const char* address = sa.ToString(false);
	sq_pushstring(vm, address, strlen(address));
	return 1;
}
SQInteger sq_playerPutToVehicle(SQVM *vm)
{
	SQInteger	ID,carID,seatID;
	sq_getinteger(vm, -3, &ID);
	sq_getinteger(vm, -2, &carID);
	sq_getinteger(vm, -1, &seatID);
	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(carID);
		if (veh != NULL)
		{
			// todo: to car fast
			BitStream bsOut;
			bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((MessageID)LHMP_PLAYER_PUT_TO_VEHICLE);
			bsOut.Write(ID);
			bsOut.Write(carID);
			bsOut.Write(seatID);
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			sq_pushbool(vm, true);
			return 1;
		}
	}
	sq_pushbool(vm, false);
	return 1;
}
SQInteger sq_playerKick(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);
	SystemAddress sa = g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID);
	g_CCore->GetNetworkManager()->GetPeer()->CloseConnection(sa, true);
	return 1;
}
SQInteger sq_playerGetPing(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);
	SystemAddress sa = g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID);
	int ping = g_CCore->GetNetworkManager()->GetPeer()->GetLastPing(sa);
	sq_pushinteger(vm, ping);
	return 1;
}
SQInteger sq_playerGetName(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);
	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		char* nick = player->GetNickname();
		sq_pushstring(vm, nick, strlen(nick));
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}


SQInteger sq_serverGetName(SQVM *vm)
{
	sq_pushstring(vm, g_CCore->GetNetworkManager()->m_pSvrName.c_str(), g_CCore->GetNetworkManager()->m_pSvrName.length());
	return 1;
}
SQInteger sq_serverGetGamemodeName(SQVM *vm)
{
	sq_pushstring(vm, g_CCore->GetNetworkManager()->m_pServerMode.c_str(), g_CCore->GetNetworkManager()->m_pServerMode.length());
	return 1;
}
SQInteger sq_serverSetGamemodeName(SQVM *vm)
{
	const SQChar* modename;
	sq_getstring(vm, -1, &modename);
	g_CCore->GetNetworkManager()->m_pServerMode = modename;
	return 1;
}
SQInteger sq_serverGetOnlinePlayers(SQVM *vm)
{
	
	sq_pushinteger(vm, g_CCore->GetNetworkManager()->GetPeer()->NumberOfConnections());
	return 1;
}
SQInteger sq_serverGetMaxPlayers(SQVM *vm)
{
	sq_pushinteger(vm, g_CCore->GetNetworkManager()->m_pServerMaxPlayers);
	return 1;
}

SQInteger sq_serverSetDefaultMap(SQVM *vm)
{
	const SQChar* map;
	sq_getstring(vm, -1, &map);
	//g_CCore->GetNetworkManager()->m_pServerMode = modename;
	g_CCore->SetDefaultMap((char*)map);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_SCRIPT_CHANGE_MAP);
	bsOut.Write(map);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	return 1;
}


SQInteger sq_playerAddWeapon(SQVM *vm)
{
	SQInteger ID,weaponID,ammo,ammoSecond;
	sq_getinteger(vm, -4, &ID);
	sq_getinteger(vm, -3, &weaponID);
	sq_getinteger(vm, -2, &ammo);
	sq_getinteger(vm, -1, &ammoSecond);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		player->AddWeapon(weaponID,ammo,ammoSecond);

		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_ADDWEAPON);
		bsOut.Write(ID);
		bsOut.Write(weaponID);
		bsOut.Write(ammo);
		bsOut.Write(ammoSecond);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	return 1;
}
SQInteger sq_playerDeleteWeapon(SQVM *vm)
{
	SQInteger ID, weaponID;
	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &weaponID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		player->DeleteWeapon(weaponID);

		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_DELETEWEAPON);
		bsOut.Write(ID);
		bsOut.Write(weaponID);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, MEDIUM_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	return 1;
}

SQInteger sq_isAnyPlayerInVehicle(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		for (int i = 0; i < 3; i++)
		{
			if (veh->GetSeat(i) != -1)
			{
				sq_pushbool(vm, true);
				return 1;
			}
		}
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_vehicleSpawn(SQVM *vm)
{
	SQInteger	modelID;
	Vector3D	pos;
	Vector3D	rot;
	sq_getinteger(vm, -7, &modelID);
	sq_getfloat(vm, -6, &pos.x);
	sq_getfloat(vm, -5, &pos.y);
	sq_getfloat(vm, -4, &pos.z);
	sq_getfloat(vm, -3, &rot.x);
	sq_getfloat(vm, -2, &rot.y);
	sq_getfloat(vm, -1, &rot.z);

	//printf("spawn vehicle, %d %f %f %f %f %f %f \n", modelID, pos.x, pos.y, pos.z, rot.x, rot.y, rot.z);
	VEH::CREATE vehicle_data;
	int ID = g_CCore->GetVehiclePool()->New();
	if (ID != -1)
	{
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
		veh->SetRespawnPosition(pos);
		veh->SetRespawnRotation(rot);
		veh->SetRespawnRotationSecond(veh->GetSecondRot());
		veh->SetPosition(pos);
		veh->SetRotation(rot);
		veh->SetSkin(modelID);

		vehicle_data.isSpawned = true;
		vehicle_data.position = pos;
		vehicle_data.rotation = rot;
		vehicle_data.skinID = modelID;
		vehicle_data.damage = veh->GetDamage();
		vehicle_data.shotdamage = veh->GetShotDamage();
		vehicle_data.roofState = veh->GetRoofState();
		vehicle_data.siren = veh->GetSirenState();

		vehicle_data.ID = ID;
		for (int i = 0; i < 4; i++)
			vehicle_data.seat[i] = -1;
		BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_CREATE);
		bsOut.Write(vehicle_data);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_RAKNET_GUID, true);

		sq_pushbool(vm, true);
	}
	else
	{
		sq_pushbool(vm, false);
	}
	return 1;
}
SQInteger sq_vehicleDelete(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);
	g_CCore->GetVehiclePool()->Delete(ID);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_VEHICLE_DELETE);
	bsOut.Write(ID);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	sq_pushbool(vm, true);
	return 1;
}

SQInteger sq_vehicleToggleRoof(SQVM *vm)
{
	SQInteger ID;
	SQInteger state;

	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &state);

	state = !state;

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);


	if (veh != NULL)
	{
		veh->ToggleRoof((byte)state);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_TOGGLE_ROOF);
		bsOut.Write(ID);
		bsOut.Write(state);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true); return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_vehicleToggleSiren(SQVM *vm)
{
	SQInteger ID;
	SQBool state;

	sq_getinteger(vm, -2, &ID);
	sq_getbool(vm, -1, &state);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);


	if (veh != NULL)
	{
		veh->SetSirenState((bool)state);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_TOGGLE_SIREN);
		bsOut.Write(ID);
		bsOut.Write(state);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true); return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_vehicleGetSirenState(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* vehicle = g_CCore->GetVehiclePool()->Return(ID);

	if (vehicle != NULL)
	{
		bool state = vehicle->GetSirenState();

		sq_pushbool(vm, state);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_vehicleGetRoofState(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* vehicle = g_CCore->GetVehiclePool()->Return(ID);

	if (vehicle != NULL)
	{
		bool state = (vehicle->GetRoofState() == 1);
		sq_pushbool(vm, !state);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_vehicleGetDriverID(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		sq_pushinteger(vm,veh->GetSeat(0));
		return 1;
	}
	sq_pushinteger(vm, -1);
	return 1;
}
SQInteger sq_vehicleGetPlayerIDFromSeat(SQVM *vm)
{
	SQInteger ID,seatID;
	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &seatID);
	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		sq_pushinteger(vm, veh->GetSeat(seatID));
		return 1;
	}
	sq_pushinteger(vm, -1);
	return 1;
}
SQInteger sq_vehicleGetPosition(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		Vector3D pos = veh->GetPosition();
		sq_newarray(vm, 0);
		sq_pushfloat(vm, pos.x);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.y);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.z);
		sq_arrayappend(vm, -2);
		sq_push(vm, -1);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_vehicleGetDamage(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		float damage = veh->GetDamage();
		sq_pushfloat(vm, damage);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_vehicleGetRotation(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		Vector3D pos = veh->GetRotation();
		sq_newarray(vm, 0);
		sq_pushfloat(vm, pos.x);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.y);
		sq_arrayappend(vm, -2);
		sq_pushfloat(vm, pos.z);
		sq_arrayappend(vm, -2);
		sq_push(vm, -1);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
SQInteger sq_vehicleSetPosition(SQVM *vm)
{
	SQInteger	ID;
	Vector3D	pos;
	sq_getinteger(vm, -4, &ID);
	sq_getfloat(vm, -3, &pos.x);
	sq_getfloat(vm, -2, &pos.y);
	sq_getfloat(vm, -1, &pos.z);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->SetPosition(pos);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_SET_POSITION);
		bsOut.Write(ID);
		bsOut.Write(pos);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(veh->GetSeat(0)), false);
		
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}
SQInteger sq_vehicleSetSpeed(SQVM *vm)
{
	SQInteger	ID;
	SQFloat		speed;
	sq_getinteger(vm, -2, &ID);
	sq_getfloat(vm, -1, &speed);

	Vector3D vSpeed, rot;

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		rot = veh->GetRotation();
		vSpeed.x = speed*rot.x;
		vSpeed.y = speed*rot.y;
		vSpeed.z = speed*rot.z;
		veh->SetSpeed(vSpeed);
		//LHMP_VEHICLE_S
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_SET_SPEED);
		bsOut.Write(ID);
		bsOut.Write(vSpeed);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}
SQInteger sq_vehicleSetDamage(SQVM *vm)
{
	SQInteger	ID;
	SQFloat		damage;
	sq_getinteger(vm, -2, &ID);
	sq_getfloat(vm, -1, &damage);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->SetDamage(damage);
		return 1;
	}
	return 1;
}
SQInteger sq_vehicleSetRotation(SQVM *vm)
{
	SQInteger	ID;
	Vector3D	rot;
	sq_getinteger(vm, -4, &ID);
	sq_getfloat(vm, -3, &rot.x);
	sq_getfloat(vm, -2, &rot.y);
	sq_getfloat(vm, -1, &rot.z);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->SetRotation(rot);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_SET_ROTATION);
		bsOut.Write(ID);
		bsOut.Write(rot);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(veh->GetSeat(0)), false);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_vehicleExplode(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
		veh->SetExplodeTime(g_CCore->GetTickManager()->GetTime());
		veh->SetExploded(true);

		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_ON_EXPLODED);
		bsOut.Write(ID);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}



SQInteger sq_playerPlayAnim(SQVM *vm)
{
	const SQChar* name;
	SQInteger ID;
	sq_getinteger(vm, -2, &ID);
	sq_getstring(vm, -1, &name);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_PLAYANIM_STRING);
	bsOut.Write(ID);
	bsOut.Write(name);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0,UNASSIGNED_SYSTEM_ADDRESS, true);

	return 1;
}

SQInteger sq_playerPlaySound(SQVM *vm)
{
	const SQChar* name; 
	SQInteger ID;
	sq_getinteger(vm, -2, &ID);
	sq_getstring(vm, -1, &name);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_PLAYSOUND_STRING);
	bsOut.Write(name);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);

	return 1;
}
SQInteger sq_allPlaySound(SQVM *vm)
{
	const SQChar* name;
	sq_getstring(vm, -1, &name);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_PLAYSOUND_STRING);
	bsOut.Write(name);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);

	return 1;
}


SQInteger sq_playerSetCameraPos(SQVM *vm)
{
	SQInteger ID;
	Vector3D pos, rot;
	//SQArray 
	sq_getinteger(vm, -7, &ID);
	sq_getfloat(vm, -6, &pos.x);
	sq_getfloat(vm, -5, &pos.y);
	sq_getfloat(vm, -4, &pos.z);
	sq_getfloat(vm, -3, &rot.x);
	sq_getfloat(vm, -2, &rot.y);
	sq_getfloat(vm, -1, &rot.z);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_SET_CAMERA);
	bsOut.Write(pos);
	bsOut.Write(rot);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
	return 1;
}
SQInteger sq_playerSetCameraToDefault(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);
	BitStream bsOut;
	bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
	bsOut.Write((MessageID)LHMP_PLAYER_SET_CAMERA_DEFAULT);
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
	return 1;
}



SQInteger sq_timerCreate(SQVM *vm)
{
	SQInteger interval,repeat;	
	const SQChar* name;
	sq_getstring(vm, -3, &name);
	sq_getinteger(vm, -2, &interval);
	sq_getinteger(vm, -1, &repeat);
	g_CCore->GetTimerPool()->New(vm, (char*)name, interval, repeat);
	return 1;
}

SQInteger sq_timerDelete(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);
	g_CCore->GetTimerPool()->Delete(ID);
	return 1;
}

SQInteger sq_pickupCreate(SQVM *vm)
{
	SQInteger interval;
	SQFloat	x, y, z,size;
	const SQChar* name;

	sq_getstring(vm, -6, &name);
	sq_getinteger(vm, -5, &interval);
	sq_getfloat(vm, -4, &x);
	sq_getfloat(vm, -3, &y);
	sq_getfloat(vm, -2, &z);
	sq_getfloat(vm, -1, &size);
	
	int ID = g_CCore->GetPickupPool()->New((char*)name,interval,x,y,z,size);
	CPickup* pickup = g_CCore->GetPickupPool()->Return(ID);
	if (pickup != NULL)
	{
		pickup->SendIt(-1);	// send it everybody
	}
	return 1;
}

// real inigetParam
void iniGetParam(const char* file, const char* param, char* value)
{
	char filepath[256] = "";
	sprintf(filepath, "gamemodes/%s/%s", g_CCore->GetGameMode()->GetName(), file);
	std::ifstream fp(filepath);
	std::string line;
	while (std::getline(fp, line))
	{
		std::istringstream iss(line);
		std::string null, tmp;

		if ((iss >> null >> tmp))
		{
			char cnull[256] = "";
			sprintf(cnull, "%s", null.c_str());
			if (!strcmp(cnull, param))
			{
				char out[256] = "";
				int offset = strlen(param) + 1;
				for (int i = 0; i < (strlen(line.c_str()) - strlen(param) - 1); i++)
				{
					out[i] = line.c_str()[i + offset];
				}
				sprintf(value, "%s", out);
				return;
			}
		}
	}
	sprintf(value, "");
	return;
}

SQInteger sq_iniGetParam(SQVM *vm)
{
	const SQChar* param;
	const SQChar* file;

	sq_getstring(vm, -1, &param);
	sq_getstring(vm, -2, &file);

	SQChar out[256];

	//g_CCore->GetFileSystem()->iniGetParam(file, param, out);
	iniGetParam(file, param, out);

	sq_pushstring(vm, _SC(out), -1);

	return 1;
}

// real iniSetParam
void iniSetParam(const char* file, const char* param, const char* value)
{
	bool isAppend = true;
	char filepath[256] = ""; 
	sprintf(filepath, "gamemodes/%s/%s", g_CCore->GetGameMode()->GetName(), file);
	std::ifstream fp(filepath);
	std::string line, buffer;
	while (std::getline(fp, line))
	{
		buffer += line + '\n'; // Populates my buffer with data from file.
	}
	fp.close();

	std::fstream fo;
	fo.open(filepath, std::ios::out);
	std::istringstream b(buffer);

	while (std::getline(b, line))
	{
		std::istringstream iss(line);
		std::string null, tmp;
		if ((iss >> null >> tmp))
		{
			char cnull[256] = "";
			sprintf(cnull, "%s", null.c_str());
			if (!strcmp(cnull, param))
			{
				char out[256] = "";
				sprintf(out, "%s %s", param, value);
				fo << std::string(out) << std::endl;
				fo.flush();
				isAppend = false;
				continue;
			}
		}
		fo << line << std::endl;
		fo.flush();
	}

	if (isAppend)
	{
		char out[256] = "";
		sprintf(out, "%s %s", param, value);
		fo << out << std::endl;
		fo.flush();
	}

	return;
}

SQInteger sq_iniSetParam(SQVM *vm)
{
	const SQChar* param;
	const SQChar* file;
	const SQChar* value;

	sq_getstring(vm, -1, &value);
	sq_getstring(vm, -2, &param);
	sq_getstring(vm, -3, &file);

	iniSetParam(file, param, value);

	return 1;
}

// real iniRemoveFile
void iniRemoveFile(const char* file)
{
	char path[256];
	sprintf(path, "gamemodes/%s/%s", g_CCore->GetGameMode()->GetName(), file);
	int ret_code = std::remove(path);
	if (ret_code != 0)
	{
		g_CCore->GetLog()->AddNormalLog("[FS] Can't remove file: %s (%d).", file, ret_code);
	}
}

SQInteger sq_iniRemoveFile(SQVM *vm)
{
	const SQChar* file;

	sq_getstring(vm, -1, &file);

	iniRemoveFile(file);

	return 1;
}


void iniCreateFile(const char* file)
{
	std::ofstream of(file);
	of << '\0';
}

SQInteger sq_iniCreateFile(SQVM *vm)
{
	const SQChar* file;

	sq_getstring(vm, -1, &file);

	iniCreateFile(file);

	return 1;
}


