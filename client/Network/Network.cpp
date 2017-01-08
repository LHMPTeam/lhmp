#include "Network.h"

Network::Network()
{
}


Network::~Network()
{
}

void Network::Init()
{
	mPeer = RakNet::RakPeerInterface::GetInstance();
	mPeer->Startup(1, &mSocketDescriptor, 1);
}

void Network::Tick()
{
	for (RakNet::Packet* packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
	{
		switch (packet->data[0])
		{
			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				OnConnectionAccepted(packet);
			}
			break;

			case MessageIDs::ID_CONNECTION_REFUSED_LHMP:
			{
				// TODO(zaklaus): Actually do something to tell the client about the issue.
				exit(-1);
			}
			break;

			case MessageIDs::ID_CONNECTION_ACCEPTED_LHMP:
			{

			}
			break;
		}
	}
}

void Network::OnConnectionAccepted(RakNet::Packet * packet)
{
	RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID)MessageIDs::ID_CONNECTION_INIT_LHMP);
	bitStream.Write(BUILD_VERSION);
	bitStream.Write(mUserName);
	mPeer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

bool Network::Connect(const char* ipAddress, int port, std::string serverPassword)
{
	auto result = 0;
	if (!serverPassword.size())
		result = mPeer->Connect(ipAddress, port, 0, 0);
	else
		result = mPeer->Connect(ipAddress, port, serverPassword.c_str(), serverPassword.size());

	return result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED;
}

