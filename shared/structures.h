/**
	Lost Heaven Multiplayer - shared
	structures.h
	Purpose: often used structures (e.g. vectors)
*/
#ifndef __STRUCTURES_H
#define	__STRUCTURES_H

#include <iostream>
#include "stdio.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include <string>
#include "RakNetStatistics.h"
#include "RakNetTime.h"
#include "GetTime.h"

#define MAX_USHORT  (unsigned short) (-1)

#define NO_CAR		(int) -1
#define NO_PLAYER	(int) -1


#ifndef _WIN32
typedef unsigned char byte;
#endif // _WIN32

struct Vector2D
{
	int x,y;
};

struct Vector3D{
	float x;
	float y;
	float z;
	Vector3D(float _x = 0.0f,float _y = 0.0f, float _z = 0.0f)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3D operator+(const Vector3D& a) const
	{
		return Vector3D(a.x + x, a.y + y,a.z+z);
	}

	Vector3D& operator=(const Vector3D&a)
	{
		x = a.x;
		y = a.y;
		z = a.z;
		return *this;
	}

};

struct Vector4D{
	float x;
	float y;
	float z;
	float w;
	Vector4D()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 0.0f;
	}
};

struct _Player{
	Vector3D playerPos;
	int iPlayerID;
	char sName[80];
	float fHealth;
	float rotation1;
	float rotation2;
	float rotation3;
	byte state;
	int iCurrAmmo;
	int iMaxAmmo;
	bool isDucking;
	int	skinID;
	bool isAim;
};

struct _Server{
	int playerid;
	int max_players;
	int server_port;
	char server_name[255];
	//Vector3D spawnPos;
};

enum GameMessages
{
	// sends client after connection accept
	ID_INITLHMP = ID_USER_PACKET_ENUM + 1,
	ID_GAME_SKUSKA = ID_USER_PACKET_ENUM+2,
	ID_GAME_SYNC = ID_USER_PACKET_ENUM+3,
	ID_GAME_PID = ID_USER_PACKET_ENUM+4,
	ID_GAME_IP = ID_USER_PACKET_ENUM+5,
	ID_GAME_ALIVE = ID_USER_PACKET_ENUM+6,
	ID_GAME_LHMP_PACKET,
	ID_GAME_BAD_VERSION,
	// when whole server-client connecting is finished and client is finaly ready to play
	ID_CONNECTION_FINISHED,
	ID_FILETRANSFER,
	ID_SERVERRELOAD,
	// indicates that server is sending the whole list of client-side scripts
	ID_SCRIPSTLIST

};

enum FileTransfer
{
	FILETRANSFER_INIT,
	FILETRANSFER_SEND,
	FILETRANSFER_FINISH
};

enum LHMPNetMessages
{
	LHMP_PLAYER_SENT_SYNC_ON_FOOT,
	LHMP_PLAYER_SENT_CAR_UPDATE,
	LHMP_PLAYER_CHAT_MESSAGE,
	LHMP_PLAYER_COMMAND,
	LHMP_PLAYER_CONNECT,
	LHMP_PLAYER_DISCONNECT,
	LHMP_PLAYER_CHANGESKIN,
	LHMP_PLAYER_PLAYANIM,
	LHMP_PLAYER_PLAYANIM_STRING,
	LHMP_PLAYER_PLAYSOUND_STRING,
	LHMP_PLAYER_RESPAWN,
	LHMP_PLAYER_PING,
	LHMP_PLAYER_ADDWEAPON,
	LHMP_PLAYER_DELETEWEAPON,
	LHMP_PLAYER_SWITCHWEAPON,
	LHMP_PLAYER_THROWGRANADE,
	LHMP_PLAYER_SHOOT,
	LHMP_PLAYER_DEATH,
	LHMP_PLAYER_DEATH_END,
	LHMP_PLAYER_HIT,
	LHMP_PLAYER_PUT_TO_VEHICLE,
	LHMP_PLAYER_KICK_OUT_VEHICLE,
	LHMP_PLAYER_SET_HEALTH,
	LHMP_PLAYER_SET_POSITION,
	LHMP_PLAYER_SET_ROTATION,
	LHMP_PLAYER_SET_CAMERA,
	LHMP_PLAYER_SET_CAMERA_DEFAULT,
	LHMP_PLAYER_SET_NAMETAG,
	LHMP_PLAYER_SET_MONEY,
	LHMP_PLAYER_ENABLE_MONEY,
	LHMP_PLAYER_TOGGLE_CITY_MUSIC,
	LHMP_PLAYER_SET_CAMERA_SWING,
	LHMP_PLAYER_SET_CAMERA_FOV,
	LHMP_PLAYER_SET_WEATHER_PARAM,
	LHMP_PLAYER_TIMER_ON,
	LHMP_PLAYER_TIMER_OFF,
	LHMP_PLAYER_SET_TIMER_INTERVAL,
	LHMP_PLAYER_GET_TIMER_INTERVAL,
	LHMP_PLAYER_SET_OBJECTIVE,
	LHMP_PLAYER_CLEAR_OBJECTIVE,
	LHMP_PLAYER_ADD_CONSOLE_TEXT,
	LHMP_PLAYER_LOCK_CONTROLS,

	// vehicles
	LHMP_PLAYER_ENTERED_VEHICLE,
	LHMP_PLAYER_EXIT_VEHICLE,
	LHMP_PLAYER_EXIT_VEHICLE_FINISH,
	LHMP_VEHICLE_CREATE,
	LHMP_VEHICLE_DELETE,
	LHMP_VEHICLE_SYNC,
	LHMP_VEHICLE_JACK,
	LHMP_VEHICLE_DAMAGE,
	LHMP_VEHICLE_SHOTDAMAGE,
	LHMP_VEHICLE_SET_POSITION,
	LHMP_VEHICLE_SET_ROTATION,
	LHMP_VEHICLE_SET_SPEED,
	LHMP_VEHICLE_SET_FUEL,
	LHMP_VEHICLE_TOGGLE_ROOF,
	LHMP_VEHICLE_TOGGLE_ENGINE,
	LHMP_VEHICLE_TOGGLE_SIREN,
	LHMP_VEHICLE_ON_EXPLODED,
	LHMP_VEHICLE_RESPAWN, 
	// door
	LHMP_DOOR_SET_STATE,
	//script
	LHMP_SCRIPT_ON_KEY_PRESSED,
	LHMP_SCRIPT_CHANGE_MAP,
	LHMP_FILE_SEND,
	LHMP_SCRIPT_CALLFUNC,
	// pickup
	LHMP_PICKUP_CREATE,
	LHMP_PICKUP_DELETE,
	LHMP_PICKUP_SETVISIBLE
};

// Reasons why player died
enum LHMPDeathReasons
{
	// Killed by gun
	DEATH_SHOT = 0x0,
	// Killed by hand or melee weapon
	DEATH_HIT = 0x1,
	// Killed by explosion (of car, granade)
	DEATH_EXPLOSION = 0x2,
	// Killed by flames (molotov & petrol pump)
	DEATH_FLAMES = 0x3,
	// Killed in car (car sink or after collision)
	DEATH_INCAR = 0x5,
	// When player falls or commit suicide (via script)
	DEATH_FALL = 0x6,
	// Getting hit by car (of another player)
	DEATH_HITBYCAR = 0x8,
	// Exploding in car (LHMP)
	DEATH_EXPLODEINCAR = 10,
	// Player sink (drown (LHMP)
	DEATH_DROWN = 11
};

// Player body parts
enum LHMPPlayerParts
{
	PLAYERPART_UNKNOWN = 0,
	PLAYERPART_RIGHTARM = 1,
	PLAYERPART_LEFTARM = 2,
	PLAYERPART_RIGHTLEG = 3,
	PLAYERPART_LEFTLEG = 4,
	PLAYERPART_BODY = 5,
	PLAYERPART_HEAD = 6
};


struct vect{
	float x;
	float y;
	float z;
};

struct Slot
{
	bool isUsed;
	RakNet::SystemAddress sa;
};

// strPlayer = server-side
struct strPlayer
{
	Vector3D		position;
	float			degree;
	float			degree_second;
	float			degree_third;
	float			health;
	byte			status;
	bool			isSpawned;
	char			nickname[255];
	bool			isDucking;
	int				skinID;
	RakNet::Time	synctime;
	bool			isAim;
};

namespace SYNC {
	struct ON_FOOT_SYNC
	{
		// unicate slot ID of player
		int				ID;
		// vector of position (3 floats)
		Vector3D		position;
		// rotation
		// 0-360 mapped into 65k states (that's the capacity of WORD)
		unsigned short	rotation;
		// health
		unsigned short	health;
		// current animation by ID (jumping, walking, etc)
		byte			animStatus;
		// states - isDucking, isAimingWithGun
		// check out PLAYERSTATES enum
		unsigned char	states;
	};

	struct ON_FOOT_SYNC_SMALL
	{
		int			ID;
		Vector3D position;
	};
	struct CHANGESKIN
	{
		int			ID;
		int			skinID;
	};
	struct IN_CAR
	{
		float	health;
		int		ID;
		bool	isAiming;
		float	aim;
	};
}
// vehicles
namespace VEH
{
	struct CREATE
	{
		bool		isSpawned;
		int			ID,skinID;
		Vector3D	position;
		Vector3D	rotation;
		int			seat[4];
		float		damage;
		byte		shotdamage;
		byte        roofState;
		bool        engineState;
		bool        siren;
	};
	struct SYNC
	{
		int ID;
		Vector3D	position;
		Vector3D	rotation;
		Vector3D	secondRot;
		Vector3D	speed;
		float		wheels;
		float		fuel;
		bool		horn;
		bool		gasOn;
	};
}

struct PlayerSync
{
	char chrNick[255];
	int  intPing;
	bool bIsUsed;
};


enum CLIENT_ENGINESTACK
{
	ES_CREATEPLAYER,
	ES_DELETEPLAYER,
	ES_PLAYERDEATH,
	ES_PLAYERDEATHEX,
	ES_PLAYERDEATH_END,
	ES_CHANGESKIN,
	ES_PLAYANIM,
	ES_PLAYANIM_STRING,
	ES_PLAYSOUND_STRING,
	ES_ADDWEAPON,
	ES_DELETEWEAPON,
	ES_SWITCHWEAPON,
	ES_SHOOT,
	ES_CAMERASETPOS,
	ES_CAMERAUNLOCK,
	// vehicles
	ES_CREATECAR,
	ES_DELETECAR,
	ES_PLAYER_PUT_TO_VEH,
	ES_PLAYER_KICK_OUT_VEHICLE,
	ES_PLAYER_ENTER_VEH,
	ES_PLAYER_EXIT_VEH,
	ES_VEHICLE_JACK,
	ES_THROWGRANADE,
	ES_CAREXPLODE,
	ES_CARRESPAWN,
	//door
	ES_DOOR_SET_STATE,
	ES_CHANGEMAP,
	//pickup
	ES_CREATEPICKUP,
	ES_DELETEPICKUP,
	ES_SETPICKUPVISIBLE,
	// intern
	ES_SERVERRELOAD,
	ES_PLAYERSETPOS,

	//---------------- SCRIPTS -----------------
	ES_SCRIPT_RUN,
	ES_SCRIPT_ONKEYDOWN
};
namespace ENGINE_STACK
{
	struct PLAYER_ADDWEAPON
	{
		PLAYER_ADDWEAPON(int _ID, int _wepID,int _wepLoaded, int _wepHidden)
		{
			this->ID = _ID;
			this->wepID = _wepID;
			this->wepLoaded = _wepLoaded;
			this->wepHidden = _wepHidden;
		}
		int ID,wepID,wepLoaded,wepHidden;
	};
	struct PLAYER_DELETEWEAPON
	{
		PLAYER_DELETEWEAPON(int _ID, int _wepID)
		{
			this->ID = _ID;
			this->wepID = _wepID;
		}
		int ID, wepID;
	};
	struct PLAYER_SWITCHWEAPON
	{
		PLAYER_SWITCHWEAPON(int _ID, int _wepID)
		{
			this->ID = _ID;
			this->wepID = _wepID;
		}
		int ID, wepID;
	};
	struct PLAYER_SHOOT
	{
		PLAYER_SHOOT(int _ID, Vector3D _pos)
		{
			this->ID = _ID;
			this->pos = _pos;
		}
		int ID;
		Vector3D pos;
	};	
	struct PLAYER_THROWGRANADE
	{
		PLAYER_THROWGRANADE(int _ID, Vector3D _pos)
		{
			this->ID = _ID;
			this->pos = _pos;
		}
		int ID;
		Vector3D pos;
	};
	struct PLAYER_ENTER_VEH
	{
		PLAYER_ENTER_VEH(int _pID, int _vehID, int _seatID)
		{
			this->pID = _pID;
			this->vehID = _vehID;
			this->seatID = _seatID;
		}
		int pID, vehID, seatID;
	};
	struct PLAYER_EXIT_VEH
	{
		PLAYER_EXIT_VEH(int _pID, int _vehID)
		{
			this->pID = _pID;
			this->vehID = _vehID;
		}
		int pID, vehID;
	};
	struct PLAYER_PLAYANIM
	{
		PLAYER_PLAYANIM(int _ID, char* _name)
		{
			this->ID = _ID;
			strcpy(name, _name);
		}
		int ID;
		char name[255];
	};
	struct PLAYER_PLAYSOUND
	{
		PLAYER_PLAYSOUND(char* _name)
		{
			strcpy(name, _name);
		}
		char name[255];
	};
	struct VEH_CREATE
	{
		VEH_CREATE(int _vehID, int _skinID)
		{
			this->vehID = _vehID;
			this->skinID = _skinID;
		}
		int vehID, skinID;
	};
	struct VEH_JACK
	{
		VEH_JACK(int _pID, int _vehID, int _seatID)
		{
			this->pID = _pID;
			this->vehID = _vehID;
			this->seatID = _seatID;
		}
		int pID, vehID,seatID;
	};

	struct CAMERA_SET
	{
		CAMERA_SET(Vector3D _pos, Vector3D _rot)
		{
			this->pos = _pos;
			this->rot = _rot;
		}
		Vector3D pos, rot;
	};
	struct VEH_DELETEVEH
	{
		VEH_DELETEVEH(int _vehID, unsigned int _base)
		{
			this->vehID = _vehID;
			this->base = _base;
		}
		int vehID;
		unsigned int base;
	};
	struct DOOR_SET_STATE
	{
		DOOR_SET_STATE(char* _buff, bool _state, bool _facing)
		{
			strcpy(buff, _buff);
			this->state = _state;
			this->facing = _facing;
		}
		char buff[200];
		bool state;
		bool facing;
	};
	struct KILL_PED_EX
	{
		KILL_PED_EX(int _ID, int _reason, int _part)
		{
			this->ID = _ID;
			this->reason = _reason;
			this->part = _part;
		}
		int ID;
		int reason;
		int part;
	};

	struct PLAYER_SETPOS
	{
		PLAYER_SETPOS(int _ID,Vector3D _pos)
		{
			this->ID = _ID;
			this->pos = _pos;
		}
		int ID;
		Vector3D pos;
	};
}

struct SWeapon
{
	int wepID, wepLoaded,wepHidden;
};

enum SCRIPT_KEYBOARD {
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_R,
	KEY_Q,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Z,
	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,
	KEY_NUM0,
	KEY_ARROWUP,
	KEY_ARROWDOWN,
	KEY_ARROWLEFT,
	KEY_ARROWRIGHT,
};
const char globalMap[500] = "verylongmapverylongmapverylongmapverylongmapverylongmapverylongmap";

enum PLAYERSTATES
{
	//if player is ducking(crouching) state
	ONFOOT_ISDUCKING,
	//if player is aiming(pointing with weapon)
	ONFOOT_ISAIMING,
	//if player is in animation with car (entering/exiting/being bumped by car)
	ONFOOT_ISCARANIM
};
#endif