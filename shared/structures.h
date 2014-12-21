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
	Vector3D()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
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
	ID_SERVERRELOAD

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
	LHMP_PLAYER_HIT,
	LHMP_PLAYER_PUT_TO_VEHICLE,
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
	LHMP_VEHICLE_TOGGLE_ROOF,
	LHMP_VEHICLE_TOGGLE_SIREN,
	LHMP_VEHICLE_ON_EXPLODED,
	LHMP_VEHICLE_RESPAWN, 
	// door
	LHMP_DOOR_SET_STATE,
	//script
	LHMP_SCRIPT_ON_KEY_PRESSED,
	LHMP_SCRIPT_CHANGE_MAP,
	LHMP_FILE_SEND,
	// pickup
	LHMP_PICKUP_CREATE,
	LHMP_PICKUP_DELETE,
	LHMP_PICKUP_SETVISIBLE
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
		int				ID;
		Vector3D		position;
		float			degree;
		float			degree_second;
		float			degree_third;
		byte			status;
		float			health;
		bool			isDucking;
		bool			isAim;
		bool			isCarAnim;
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
	ES_SERVERRELOAD
};
namespace ENGINE_STACK
{
	struct PLAYER_ADDWEAPON
	{
		int ID,wepID,wepLoaded,wepHidden;
	};
	struct PLAYER_DELETEWEAPON
	{
		int ID, wepID;
	};
	struct PLAYER_SWITCHWEAPON
	{
		int ID, wepID;
	};
	struct PLAYER_SHOOT
	{
		int ID;
		Vector3D pos;
	};	
	struct PLAYER_THROWGRANADE
	{
		int ID;
		Vector3D pos;
	};
	struct PLAYER_ENTER_VEH
	{
		int pID, vehID, seatID;
	};
	struct PLAYER_EXIT_VEH
	{
		int pID, vehID;
	};
	struct PLAYER_PLAYANIM
	{
		int ID;
		char name[255];
	};
	struct PLAYER_PLAYSOUND
	{
		char name[255];
	};
	struct VEH_CREATE
	{
		int vehID, skinID;
	};
	struct VEH_JACK
	{
		int pID, vehID,seatID;
	};

	struct CAMERA_SET
	{
		Vector3D pos, rot;
	};
	struct VEH_DELETEVEH
	{
		int vehID;
		unsigned int base;
	};
	struct DOOR_SET_STATE
	{
		char buff[200];
		bool state;
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
/*const char *mapNames[] = {
	"freeride",
	"00menu",
	"intermezzo02",
	"intermezzo03",
	"mise01",
	"mise02a-taxi",
	"mise02-saliery",
	"mise02-ulicka",
	"mise03-saliery",
	"mise03-morello",
	"mise03-salierykonec",
	"mise04-krajina",
	"mise04-mesto",
	"mise04-motorest",
	"mise04-saliery",
	"mise05-mesto",
	"mise05-saliery",
	"mise06-autodrom",
	"mise06-mesto",
	"mise06-oslava",
	"mise06-saliery",
	"mise07b-chuligani",
	"mise07b-saliery",
	"mise07-saliery",
	"mise07-sara",
	"mise08-hotel",
	"mise08-kostel",
	"mise08-mesto",
	"mise09-krajina",
	"mise09-mesto",
	"mise09-prejimka",
	"mise09-saliery",
	"mise10-letiste",
	"mise10-mesto",
	"mise10-saliery",
	"mise11-mesto",
	"mise11-saliery",
	"mise11-vila",
	"mise12-saliery",
	"mise12-garage",
	"mise12-mesto",
	"mise13-mesto",
	"mise13-mesto2",
	"mise13-restaurace",
	"mise13-zradce",
	"mise14-mesto",
	"mise14-parnik",
	"mise14-saliery",
	"mise15-mesto",
	"mise15-pristav",
	"mise15-saliery",
	"mise16-krajina",
	"mise16-letiste",
	"mise16-mesto",
	"mise16-saliery",
	"mise17-saliery",
	"mise17-mesto",
	"mise17-vezeni",
	"mise18-mesto",
	"mise18-pristav",
	"mise18-saliery",
	"mise19-banka",
	"mise19-mesto",
	"mise19-pauli",
	"mise20-galery",
	"mise20-mesto",
	"mise20-pauli",
	"freeridenoc",
	"freeride",
	"freekrajina",
	"freekrajinanoc",
	"carcyclopedia"

};*/

#endif