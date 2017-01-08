#include <stdinc.h>

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
			case MessageIDs::ID_CONNECTION_REFUSED_LHMP:
			case MessageIDs::ID_CONNECTION_ACCEPTED_LHMP:
			{
				ClientConnectionHandler clientHandler;
				clientHandler.ProcessMessage(this, packet);
			}
			break;
		}
	}
}

bool Network::Connect(const char* ipAddress, int port, std::string serverPassword) const
{
	auto result = 0;
	if (!serverPassword.size())
		result = mPeer->Connect(ipAddress, port, 0, 0);
	else
		result = mPeer->Connect(ipAddress, port, serverPassword.c_str(), serverPassword.size());

	return result == RakNet::ConnectionAttemptResult::CONNECTION_ATTEMPT_STARTED;
}

