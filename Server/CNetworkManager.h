#ifndef CNETWORKMANAGER_H
#define CNETWORKMANAGER_H

#include "../shared/structures.h"
#include "RakPeerInterface.h"
#include "MessageIdentifiers.h"
#include "RakSleep.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include <stdlib.h>
#ifndef _WIN32
typedef unsigned long DWORD;
#endif // _WIN32

//#include "CCore.h"
using namespace RakNet;
class CNetworkManager
{
private:
	RakPeerInterface			 *peer;
	Packet						*packet;
	//SystemAddress				*m_pPlayerID;
	Slot						*slot;
	SocketDescriptor			*sd;
	int							m_pServerPort;
	std::string					m_pStartPos;
	Vector3D					m_pSpawnPos;
	DWORD						SPUtime;
public:
	CNetworkManager();
	~CNetworkManager();
	bool						Init(int port,int players, std::string,std::string);
	int							GetIDFromSystemAddress(SystemAddress);
	SystemAddress				GetSystemAddressFromID(int);
	Slot*						GetSlotID(int ID);
	int							GetFirstFreeSlot();
	void						Pulse();
	void						LHMPPacket(Packet*, RakNet::TimeMS timestamp = NULL);
	void						SendSYNC();
	void						SendCarSYNC();
	void						SendHimOthers(int);
	void						SendHimCars(int);
	void						SendHimDoors(int);
	void						SendHimPickups(int);
	void						PostMasterlist(bool);
	void						UpdateMasterlist();
	std::string					m_pSvrName;
	int							m_pServerMaxPlayers;
	std::string					m_pServerMode;
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
