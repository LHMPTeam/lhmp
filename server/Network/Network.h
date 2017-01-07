#pragma once
#include <RakPeerInterface.h>
#include "MessageIdentifiers.h"

class Network
{
public:
	Network();
	~Network();
	void Init();
	void Tick();
	RakNet::RakPeerInterface* GetPeer()
	{
		return mPeer;
	}
private:
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor* mSocketDescriptor;
};

