#pragma once
#include "../Core.h"
#include "Client.h"
#include "MessageHandler.h"

#include <MessageIdentifiers.h>
#include <RakPeerInterface.h>
#include <BitStream.h>

#include <map>

class Network
{
public:
	Network();
	~Network();
	void Init();
	void Tick();

	RakNet::RakPeerInterface* GetPeer() { return mPeer; }
private:
	std::map<RakNet::RakNetGUID, Client> mClients;
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
};

