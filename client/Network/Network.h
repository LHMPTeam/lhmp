#pragma once

class Network
{
public:
	Network();
	~Network();
	void Init();
	void Tick();
	bool Connect(const char* ipAddress, int port, std::string serverPassword = "") const;

	RakNet::RakPeerInterface* GetPeer() const { return mPeer; }
	std::string GetNickName() const { return mUserName; }
	bool IsRunning() { return mIsRunning; }
	bool IsReadySync() { return mIsReadySync; }
	void SetSyncReady(bool value) { mIsReadySync = value; }
	void SendPlayerOnFoot(const OnFootSync& footSync);
	void SetServerAddress(RakNet::SystemAddress systemAddress) { mServerAddress = systemAddress; }
private:
	std::string mUserName;
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
	bool mIsRunning;
	bool mIsReadySync;
	RakNet::SystemAddress mServerAddress;
	std::map<RakNet::RakNetGUID, Player*> mPlayers;
};
