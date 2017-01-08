#include "Network.h"
#include <MessageIDs.h>
#include <BuildVersion.h>

Network::Network()
{
}


Network::~Network()
{
	delete mSocketDescriptor;
}

void Network::Init()
{
	ServerProperties serverProperies = Core::GetCore()->GetServerProperties();
	mSocketDescriptor = new RakNet::SocketDescriptor(serverProperies.mServerPort, 0);
	mPeer = RakNet::RakPeerInterface::GetInstance();

	if (mPeer->Startup(1, mSocketDescriptor, 1) != RakNet::RAKNET_STARTED)
	{
		Core::GetCore()->Log("Unable to startup server !\n Port might be already being used by another process !");
	}

	if (serverProperies.mPassword.size() > 0)
	{
		mPeer->SetIncomingPassword(serverProperies.mPassword.c_str(), serverProperies.mPassword.size());
	}
	
	mPeer->SetMaximumIncomingConnections(serverProperies.mMaxPlayers);
}

void Network::Tick()
{
	for (RakNet::Packet* packet = mPeer->Receive(); packet; mPeer->DeallocatePacket(packet), packet = mPeer->Receive())
	{
		switch (packet->data[0])
		{
			case ID_DISCONNECTION_NOTIFICATION:
			case ID_CONNECTION_LOST:
			{
				OnClientDisconnect(packet);
			}
			break;
			
	
			case MessageIDs::ID_CONNECTION_INIT_LHMP:
			{
				OnClientConnect(packet);
			}
			break;
		
		}
	}
}

void Network::OnClientConnect(RakNet::Packet * packet)
{
	RakNet::BitStream inStream;

	int clientVersion = -1;
	inStream.Read(clientVersion);

	RakNet::BitStream outStream;

	if (clientVersion != BUILD_VERSION)
	{
		outStream.Write((RakNet::MessageID)MessageIDs::ID_CONNECTION_REFUSED_VERSION_LHMP);
		outStream.Write(BUILD_VERSION);

		mPeer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
		return;
	}

	std::string nickName;
	inStream.Read(nickName);

	Client client(nickName, packet->systemAddress);

	mClients.insert(std::make_pair(packet->guid, client));

	outStream.Write((RakNet::MessageID)MessageIDs::ID_CONNECTION_ACCEPTED_LHMP);
	mPeer->Send(&outStream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
}

void Network::OnClientDisconnect(RakNet::Packet * packet)
{

}

