#pragma once

class Network
{
public:
	Network();
	~Network();
	void Init();
	void Tick();
	bool Connect(const char* ipAddress, int port, std::string serverPassword = "") const;

	std::string GetNickName() const { return mUserName; }
	bool IsRunning() { return mIsRunning; }
	void SetServerAddress(RakNet::SystemAddress systemAddress) { mServerAddress = systemAddress; }
	RakNet::SystemAddress GetServerAddress() const { return mServerAddress; }
	RakNet::RakPeerInterface* GetPeer() const { return mPeer; }
	std::map<RakNet::RakNetGUID, Player*> GetPlayers() { return mPlayers; }
	short GetTickRate() const { return mTickRate; }
private:
	std::string mUserName;
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
	bool mIsRunning;
	RakNet::SystemAddress mServerAddress;
	std::map<RakNet::RakNetGUID, Player*> mPlayers;
	short mTickRate;
};
