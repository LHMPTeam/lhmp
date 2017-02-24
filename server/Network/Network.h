#pragma once

class Network
{
public:
	Network();
	~Network();
	void Init();
	void Tick();
	short GetTickRate() { return mTickRate; }

	RakNet::RakPeerInterface* GetPeer() { return mPeer; }
private:
	std::map<RakNet::RakNetGUID, Client*> mClients;
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
	short mTickRate;
};

