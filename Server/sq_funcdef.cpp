#include "sq_funcdef.h"
#include "CCore.h"

#include <fstream>
#include <sstream>
#include <ios>
#include "../shared/tools.h"
#include "../sdks/sqlite/sqlite3.h"
#include "../sdks/sqlite/SQLiteResultPool.h"

extern CCore* g_CCore;

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
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
		player->SetLocked((shouldLock == 1));
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
		return 1;
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
	bsOut.Write((unsigned short)strlen(pMessage));
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
	bsOut.Write((unsigned short)strlen(pMessage));
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
	if (player != NULL && player->GetCurrentCar())
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
		sq_pushfloat(vm, player->GetFloatRotation());
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_playerGetRotationAsVector(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		float rot = player->GetFloatRotation();
		Vector3D pos = Tools::ComputeOffsetDegrees(rot);
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
		sq_pushinteger(vm, player->GetCurrentCar());
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
			//g_CCore->GetScripts()->onPlayerIsKilled(ID, ID,DEATH_FALL,0);
			BitStream bsOut;
			bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((MessageID)LHMP_PLAYER_DEATH);
			bsOut.Write(ID); 
			bsOut.Write((int)6);
			bsOut.Write((int)0);
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			if (player->GetCurrentCar() != NO_CAR)
			{
				g_CCore->GetVehiclePool()->Return(player->GetCurrentCar())->PlayerExit(ID);
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
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
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
	SQFloat rotation;
	sq_getinteger(vm, -2, &ID);
	sq_getfloat(vm, -1, &rotation);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		Vector3D newRotation = Tools::ComputeOffsetDegrees(rotation);
		player->SetRotation(newRotation);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_PLAYER_SET_ROTATION);
		bsOut.Write(ID);
		bsOut.Write(newRotation);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_playerSetRotationVector(SQVM *vm)
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
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
			sq_pushbool(vm, true);
			return 1;
		}
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_playerKickOutVehicle(SQVM *vm)
{
	SQInteger	ID, carID;
	sq_getinteger(vm, -1, &ID);
	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		carID = player->GetCurrentCar();
		CVehicle* veh = g_CCore->GetVehiclePool()->Return(carID);
		if (veh != NULL)
		{
			// todo: to car fast
			BitStream bsOut;
			bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
			bsOut.Write((MessageID)LHMP_PLAYER_KICK_OUT_VEHICLE);
			bsOut.Write(ID);
			bsOut.Write(carID);
			g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
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

SQInteger sq_playerSetNickColor(SQVM *vm)
{
	SQInteger	ID, color;
	sq_getinteger(vm, -2, &ID);
	sq_getinteger(vm, -1, &color);

	CPlayer* player = g_CCore->GetPlayerPool()->Return(ID);
	if (player != NULL)
	{
		player->net_ChangeNickColor(color);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}


SQInteger sq_serverGetName(SQVM *vm)
{
	sq_pushstring(vm, g_CCore->GetNetworkManager()->GetServerName().c_str(), g_CCore->GetNetworkManager()->GetServerName().length());
	return 1;
}
SQInteger sq_serverGetGamemodeName(SQVM *vm)
{
	sq_pushstring(vm, g_CCore->GetNetworkManager()->GetServerMode().c_str(), g_CCore->GetNetworkManager()->GetServerMode().length());
	return 1;
}
SQInteger sq_serverSetGamemodeName(SQVM *vm)
{
	const SQChar* modename;
	sq_getstring(vm, -1, &modename);
	g_CCore->GetNetworkManager()->SetServerMode(modename);
	return 1;
}
SQInteger sq_serverGetOnlinePlayers(SQVM *vm)
{
	
	sq_pushinteger(vm, g_CCore->GetNetworkManager()->GetPeer()->NumberOfConnections());
	return 1;
}
SQInteger sq_serverGetMaxPlayers(SQVM *vm)
{
	sq_pushinteger(vm, g_CCore->GetNetworkManager()->GetMaxServerPlayers());
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
	g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
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
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
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
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(ID), false);
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

SQInteger sq_getDistanceBetween3DPoints(SQVM *vm)
{
	Vector3D pointA, pointB;
	sq_getfloat(vm, -6, &pointA.x);
	sq_getfloat(vm, -5, &pointA.y);
	sq_getfloat(vm, -4, &pointA.z);
	sq_getfloat(vm, -3, &pointB.x);
	sq_getfloat(vm, -2, &pointB.y);
	sq_getfloat(vm, -1, &pointB.z);

	float c = (pointA.x - pointB.x)*(pointA.x - pointB.x) + (pointA.y - pointB.x)*(pointA.y - pointB.y) 
			+ (pointA.z - pointB.z)*(pointA.z - pointB.z);
	
	c = sqrt(abs(c));

	if (c != c) c = 0;

	sq_pushfloat(vm,c);
	return 1;
}

SQInteger sq_getDistanceBetween2DPoints(SQVM *vm)
{
	float pointAx,pointAy,pointBx,pointBy;
	sq_getfloat(vm, -6, &pointAx);
	sq_getfloat(vm, -5, &pointAy);
	sq_getfloat(vm, -3, &pointBx);
	sq_getfloat(vm, -2, &pointBy);

	float c = (pointAx - pointBx)*(pointAx - pointBx) + (pointAy - pointBx)*(pointAy - pointBy);

	c = sqrt(abs(c));

	if (c != c) c = 0;

	sq_pushfloat(vm, c);
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
		vehicle_data.engineState = (veh->GetEngineState() == 1);
		vehicle_data.siren = veh->GetSirenState();

		vehicle_data.ID = ID;
		for (int i = 0; i < 4; i++)
			vehicle_data.seat[i] = -1;
		BitStream bsOut;
		bsOut.Write((RakNet::MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((RakNet::MessageID)LHMP_VEHICLE_CREATE);
		bsOut.Write(vehicle_data);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_RAKNET_GUID, true);

		//sq_pushbool(vm, true);
		sq_pushinteger(vm, ID);
	}
	else
	{
		sq_pushinteger(vm, -1);
	}
	return 1;
}
SQInteger sq_vehicleDelete(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);
	if (g_CCore->GetVehiclePool()->Return(ID) != NULL)
	{
		g_CCore->GetVehiclePool()->Delete(ID);
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_DELETE);
		bsOut.Write(ID);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
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
		veh->SetSirenState((state == SQTrue));
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

SQInteger sq_vehicleSetFuel(SQVM *vm)
{
	SQInteger	ID;
	SQFloat		fuel;
	sq_getinteger(vm, -2, &ID);
	sq_getfloat(vm, -1, &fuel);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);

	if (veh != NULL)
	{
		veh->SetFuel(fuel);

		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_SET_FUEL);
		bsOut.Write(ID);
		bsOut.Write(fuel);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(veh->GetSeat(0)), false);
		return 1;
	}
	return 1;
}

SQInteger sq_vehicleGetFuel(SQVM *vm)
{
	SQInteger ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* vehicle = g_CCore->GetVehiclePool()->Return(ID);

	if (vehicle != NULL)
	{
		float fuel = vehicle->GetFuel();
		sq_pushfloat(vm, fuel);
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
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_VEHICLE_DAMAGE);
		bsOut.Write(ID);
		bsOut.Write(damage);
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
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
		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

SQInteger sq_vehicleExists(SQVM *vm)
{
	SQInteger	ID;
	sq_getinteger(vm, -1, &ID);

	CVehicle* veh = g_CCore->GetVehiclePool()->Return(ID);
	if (veh != NULL)
	{
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
	SQInteger interval,repeat,param;	
	const SQChar* name;
	sq_getstring(vm, -4, &name);
	sq_getinteger(vm, -3, &interval);
	sq_getinteger(vm, -2, &repeat);
	sq_getinteger(vm, -1, &param);
	int ID = g_CCore->GetTimerPool()->New(vm, (char*)name, interval, repeat,param);
	sq_pushinteger(vm, ID);
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
	sq_pushinteger(vm, ID);
	return 1;
}

SQInteger sq_pickupDelete(SQVM *vm)
{
	SQInteger pickupID;
	sq_getinteger(vm, -1, &pickupID);
	CPickup* pickup = g_CCore->GetPickupPool()->Return(pickupID);
	if (pickup != NULL)
	{
		g_CCore->GetPickupPool()->Delete(pickupID);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushbool(vm, false);
	return 1;
}

// real inigetParam
void iniGetParam(const char* file, const char* param, char* value, const char* defaultValue)
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
				for (unsigned int i = 0; i < (strlen(line.c_str()) - strlen(param) - 1); i++)
				{
					out[i] = line.c_str()[i + offset];
				}
				sprintf(value, "%s", out);
				return;
			}
		}
	}
	sprintf(value, "%s", defaultValue);
	return;
}

SQInteger sq_iniGetParam(SQVM *vm)
{
	const SQChar* param;
	const SQChar* file;
	const SQChar* def;

	sq_getstring(vm, -1, &def);
	sq_getstring(vm, -2, &param);
	sq_getstring(vm, -3, &file);


	SQChar out[256];

	//g_CCore->GetFileSystem()->iniGetParam(file, param, out);
	iniGetParam(file, param, out, def);
	sq_pushstring(vm, _SC(out), -1);

	return 1;
}

SQInteger sq_iniFileExists(SQVM *vm)
{
	const SQChar* file;

	sq_getstring(vm, -1, &file);


	char filepath[256] = "";
	sprintf(filepath, "gamemodes/%s/%s", g_CCore->GetGameMode()->GetName(), file);

	std::ifstream infile(filepath);
	sq_pushbool(vm, infile.good());
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

SQInteger sq_include(SQVM *vm)
{
	const SQChar* file;

	sq_getstring(vm, -1, &file);
	char path[256];
	sprintf(path, "gamemodes/%s/%s", g_CCore->GetGameMode()->GetName(), file);
	sqstd_dofile(vm, path, SQFalse, SQTrue);
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


SQInteger sq_callClientFunc(SQVM *vm)
{
	SQInteger	playerID;
	const SQChar* script_name,*func_name;

	sq_getinteger(vm, -4, &playerID);
	sq_getstring(vm, -3, &script_name);
	sq_getstring(vm, -2, &func_name);

	SQObjectType type_of_param = sq_gettype(vm, -1);

	// check if player exists
	CPlayer* player = g_CCore->GetPlayerPool()->Return(playerID);
	if (player != NULL)
	{
		// if does, serialize a message 
		BitStream bsOut;
		bsOut.Write((MessageID)ID_GAME_LHMP_PACKET);
		bsOut.Write((MessageID)LHMP_SCRIPT_CALLFUNC);
		bsOut.Write(script_name);
		bsOut.Write(func_name);

		// now get type of param and serialize it
		SQInteger iPar;
		SQFloat fPar;
		const SQChar* stringPar;
		SQBool	bPar;

		bsOut.Write(type_of_param);
		switch (type_of_param)
		{
		case OT_INTEGER:
			sq_getinteger(vm, -1, &iPar);
			bsOut.Write(iPar);
			break;		
		case OT_FLOAT:
			sq_getfloat(vm, -1, &fPar);
			bsOut.Write(fPar);
			break;
		case OT_STRING:
			sq_getstring(vm, -1, &stringPar);
			bsOut.Write(stringPar);
			break; 
		case OT_BOOL:
			sq_getbool(vm, -1, &bPar);
			bsOut.Write(bPar);
			break;
		case OT_NULL:
			// if null, no extra data are sent
		default:
		case -1:
			// in case bad type occurs, do nothing
			break;
		}

		g_CCore->GetNetworkManager()->GetPeer()->Send(&bsOut, IMMEDIATE_PRIORITY, RELIABLE, 0, g_CCore->GetNetworkManager()->GetSystemAddressFromID(playerID), false);
	}

	
	return 1;
}

SQInteger sq_callFunc(SQVM *vm)
{
	bool result = g_CCore->GetScripts()->callFunc(vm);
	sq_pushbool(vm, result);
	return 1;
}


SQInteger sq_sqlite3_query(SQVM *vm)
{
	sqlite3 *db;
	sqlite3_stmt *res;
	const SQChar* database;
	const SQChar* query;

	sq_getstring(vm, -2, &database);
	sq_getstring(vm, -1, &query);

	int error = sqlite3_open(database, &db);

	if (error)
	{
		printf("[SQLITE] Error while openning %s database !\n", database);
		sqlite3_close(db);
		sq_pushinteger(vm, -1);
		return 1;
	}

	error = sqlite3_prepare_v2(db, query, strlen(query), &res, NULL);

	if (error != SQLITE_OK)
	{
		printf("[SQLITE] Error while executing query %s in %s database !\n", query, database);
		printf("[SQLITE] Error: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		sq_pushinteger(vm, -1);
		return 1;
	}

	SQLiteQueryPool.push_back(res);
	int idOfResult = SQLiteResultPool_GetResultID(res);

	sq_pushinteger(vm, idOfResult);

	sqlite3_close(db);
	return 1;
}

SQInteger sq_sqlite3_step(SQVM *vm)
{
	SQInteger resultid;
	sq_getinteger(vm, -1, &resultid);

	if (resultid != -1)
	{
		sqlite3_stmt *res = SQLiteQueryPool[resultid];
		sq_pushinteger(vm, sqlite3_step(res));
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_sqlite3_column_count(SQVM *vm)
{
	SQInteger resultid;
	sq_getinteger(vm, -1, &resultid);

	if (resultid != -1)
	{
		sqlite3_stmt *res = SQLiteQueryPool[resultid];
		sq_pushinteger(vm, sqlite3_column_count(res));
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_sqlite3_finalize(SQVM *vm)
{
	SQInteger resultid;
	sq_getinteger(vm, -1, &resultid);

	if (resultid != -1)
	{
		sqlite3_stmt *res = SQLiteQueryPool[resultid];
		sqlite3_finalize(res);
		SQLiteQueryPool.erase(SQLiteQueryPool.begin(), SQLiteQueryPool.begin() + resultid);
		sq_pushbool(vm, true);
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_sqlite3_column_name(SQVM *vm)
{
	SQInteger resultid;
	SQInteger index;

	sq_getinteger(vm, -2, &resultid);
	sq_getinteger(vm, -1, &index);

	if (resultid != -1 && index != -1)
	{
		sqlite3_stmt *res = SQLiteQueryPool[resultid];

		const char* returnValue = sqlite3_column_name(res, index);
		sq_pushstring(vm, returnValue, strlen(returnValue));
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}

SQInteger sq_sqlite3_column_text(SQVM *vm)
{
	SQInteger resultid;
	SQInteger index;

	sq_getinteger(vm, -2, &resultid);
	sq_getinteger(vm, -1, &index);

	if (resultid != -1 && index != -1)
	{
		sqlite3_stmt *res = SQLiteQueryPool[resultid];
		std::string test = (char*)sqlite3_column_text(res, index);
		sq_pushstring(vm, test.c_str(), test.length());
		return 1;
	}
	sq_pushnull(vm);
	return 1;
}
