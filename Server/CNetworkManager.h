/**
Lost Heaven Multiplayer

Purpose: handles network communication

@author Romop5
@version 1.0 1/9/14
*/

#ifndef CNETWORKMANAGER_H
#define CNETWORKMANAGER_H

#include "../shared/structures.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "RakSleep.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include <stdlib.h>

#include "../shared/linux.h"

//#include "CCore.h"
using namespace RakNet;
class CNetworkManager
{
private:
	RakPeerInterface			 *peer;
	Packet						*packet;
	Slot						*slot;
	SocketDescriptor			*sd;
	int							m_pServerPort;

	std::string					m_pSvrName;
	int							m_pServerMaxPlayers;
	std::string					m_pServerMode;
	char						website[512];
public:
	CNetworkManager();
	~CNetworkManager();
	bool						Init(int port,int players, std::string,std::string);
	int							GetIDFromSystemAddress(SystemAddress);
	SystemAddress				GetSystemAddressFromID(int);
	Slot*						GetSlotID(int ID);
	int							GetFirstFreeSlot();

	void						SetServerName(std::string);
	std::string					GetServerName();
	void						SetMaxServerPlayers(int);
	int							GetMaxServerPlayers();
	void						SetServerMode(std::string);
	std::string					GetServerMode();

	void						SetWebsite(char*);
	char*						GetWebsite();

	unsigned int				GetPlayerCount();
	unsigned int				GetMaxPlayerCount();

	// Tick callback
	void						Pulse();
	// Handles all packets with ID_GAME_LHMP_PACKET
	void						LHMPPacket(Packet*, RakNet::TimeMS timestamp = NULL);

	// Tick onfoot sync
	void						SendSYNC();
	// Tick car sync
	void						SendCarSYNC();
	// Sends initial info about players to new connected player
	void						SendHimOthers(int);
	// Sends initial info about cars to new connected player
	void						SendHimCars(int);
	// Sends initial info about doors to new connected player
	void						SendHimDoors(int);
	// Sends initial info about pickups to new connected player
	void						SendHimPickups(int);

	void						UpdateConsoleName();

	void						SendMessageToAll(char*);

	void						SendPingUpdate();

	RakNet::RakPeerInterface*	GetPeer();
	int							GetPort();

	//----------------------- Callbacks ---------------------------//

	// called when client sends ID_LHMPINIT messages (containing game version etc.)
	void	OnPlayerConnection(RakNet::Packet* packet);
	void	OnPlayerFileTransferFinished(RakNet::SystemAddress);
	void	OnPlayerDisconnect(RakNet::Packet* packet);

};
#endif
