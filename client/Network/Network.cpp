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
	mPeer->Startup(1, mSocketDescriptor, 1);

}

void Network::Tick()
{
	for (RakNet::Packet* packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
	{
		switch (packet->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
			OnConnectionAccepted(packet);
			break;
		}
	}
}

void Network::OnConnectionAccepted(RakNet::Packet * packet)
{
	RakNet::BitStream bitStream;
	bitStream.Write((RakNet::MessageID)MessageIDs::ID_INIT_CLIENT);
	bitStream.Write(BUILD_VERSION);
	bitStream.Write(mUserName);
	mPeer->Send(&bitStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

bool Network::ConnectToServer(char* ipAddress, int port, char* password)
{
	mPeer->Connect(ipAddress, port, password, strlen(password));
	return false;
}

