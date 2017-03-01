#include <stdinc.h>

Network::Network()
	: mIsRunning(false),
	mIsConnected(false),
	mUserName(L"Player"),
	mTickRate(64),
	mLastMessageTime(RakNet::GetTimeMS())
{
	
}

Network::~Network()
{
}

void Network::Init()
{
	mPeer = RakNet::RakPeerInterface::GetInstance();
	mPeer->Startup(1, &mSocketDescriptor, 1);
	mIsRunning = true;
}

void Network::Tick()
{
	for (RakNet::Packet* packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
	{
		switch (packet->data[0])
		{
			case MessageIDs::LHMPID_CONNECTION:
			{
				ClientConnectionHandler clientHandler(&mPlayers);
				clientHandler.ProcessMessage(this, packet);
			}
			break;

			case MessageIDs::LHMPID_SYNC:
			{
				ClientSyncHandler clientHandler(&mPlayers);
				clientHandler.ProcessMessage(this, packet);

				mLastMessageTime = RakNet::GetTimeMS();
			}
			break;
			
			case MessageIDs::LHMPID_PLAYER:
			{
				ClientPlayerHandler clientHandler(&mPlayers);
				clientHandler.ProcessMessage(this, packet);
			}
			break;
			
			default:
				ClientRakNetHandler clientHandler(&mPlayers);
				clientHandler.ProcessMessage(this, packet);
			break;
		}
	}
}

bool Network::Connect(const char* ipAddress, int port, std::string serverPassword)
{
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Trying to connect", 0xFF0000);
	mConnectingServerAddress = ipAddress;

	auto result = 0;
	if (!serverPassword.size())
		result = mPeer->Connect(ipAddress, port, 0, 0);
	else
		result = mPeer->Connect(ipAddress, port, serverPassword.c_str(), serverPassword.size());

	return (result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED);
}