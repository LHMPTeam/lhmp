#pragma once
#include <RakPeerInterface.h>
#include <BitStream.h>
#include <MessageIdentifiers.h>
#include <MessageIDs.h>
#include <string>
#include <BuildVersion.h>

class Network
{
public:
	Network();
	~Network();
	void Init();
	void Tick();
	void OnConnectionAccepted(RakNet::Packet* packet);
	bool Connect(const char* ipAddress, int port, std::string serverPassword = "");

	RakNet::RakPeerInterface* GetPeer()
	{
		return mPeer;
	}
private:
	std::string mUserName;
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
};

