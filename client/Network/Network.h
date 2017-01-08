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
	bool Connect(const char* ipAddress, int port, std::string serverPassword = "") const;

	RakNet::RakPeerInterface* GetPeer() const { return mPeer; }
	std::string GetUserName() const { return mUserName; }
private:
	std::string mUserName;
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
};

