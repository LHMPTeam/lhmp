#pragma once

class Network
{
public:
	Network();
	~Network();
	void Init();
	void Tick();
	bool Connect(const char* ipAddress, int port, std::string serverPassword = "");
	std::string GetServerConnectIP() const { return mConnectingServerAddress; };
	std::wstring GetNickName() const { return mUserName; }
	bool IsRunning() const { return mIsRunning; }
	bool IsConnected() const { return mIsConnected; }
	void SetIsConnected(bool connected) { mIsConnected = connected;  }
	short GetTickRate() const { return mTickRate; }
	RakNet::TimeMS GetLastMessageTime() const { return mLastMessageTime; }
	void SetServerAddress(RakNet::SystemAddress systemAddress) { mServerAddress = systemAddress; }

	RakNet::SystemAddress GetServerAddress() const { return mServerAddress; }
	RakNet::RakPeerInterface* GetPeer() const { return mPeer; }
	std::map<RakNet::RakNetGUID, Player*> GetPlayers() { return mPlayers; }
private:
	RakNet::RakPeerInterface* mPeer;
	RakNet::SocketDescriptor mSocketDescriptor;
	RakNet::SystemAddress mServerAddress;
	std::map<RakNet::RakNetGUID, Player*> mPlayers;
	std::wstring mUserName;
	std::string mConnectingServerAddress;

	RakNet::TimeMS mLastMessageTime;
	bool mIsRunning;
	bool mIsConnected;
	short mTickRate;
};
