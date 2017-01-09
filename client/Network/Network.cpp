#include <stdinc.h>

Network::Network()
	: mIsRunning(false), mIsReadySync(false), mUserName("Player")
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
			case ID_CONNECTION_REQUEST_ACCEPTED:
			case MessageIDs::ID_CONNECTION_REFUSED_LHMP:
			case MessageIDs::ID_CONNECTION_ACCEPTED_LHMP:
			case MessageIDs::ID_CREATE_PLAYER:
			{
				//printf("Network Tick Connection!\n");

				ClientConnectionHandler clientHandler(&mPlayers);
				clientHandler.ProcessMessage(this, packet);
			}
			break;
			case MessageIDs::ID_SYNC_LHMP:
			{
				//printf("Network Tick Sync!\n");

				ClientSyncHandler syncHandler(&mPlayers);
				syncHandler.ProcessMessage(this, packet);
			}
			break;
		}
	}
}

bool Network::Connect(const char* ipAddress, int port, std::string serverPassword) const
{
	MafiaSDK::GetMission()->GetGame()->GetIndicators()->ConsoleAddText("Trying to connect", 0xFF0000);
	auto result = 0;
	if (!serverPassword.size())
		result = mPeer->Connect(ipAddress, port, 0, 0);
	else
		result = mPeer->Connect(ipAddress, port, serverPassword.c_str(), serverPassword.size());

	return result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED;
}

void Network::SendPlayerOnFoot(const OnFootSync& footSync)
{
	////printf("Sending foot");
	RakNet::BitStream bitStream;
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::ID_SYNC_LHMP));
	bitStream.Write(static_cast<RakNet::MessageID>(MessageIDs::LHMP_PLAYER_ONFOOTSYNC));
	bitStream.Write(footSync);
	mPeer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, mServerAddress, false);
}