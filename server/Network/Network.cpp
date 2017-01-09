#include "Network.h"
#include <MessageIDs.h>
#include <BuildVersion.h>
#include "Handlers/ServerConnectionHandler.h"
#include "Handlers/ServerSyncHandler.h"

Network::Network()
{
}


Network::~Network()
{

}

void Network::Init()
{
	ServerProperties serverProperies = Core::GetCore()->GetServerProperties();
	mSocketDescriptor = RakNet::SocketDescriptor(serverProperies.mServerPort, 0);
	mPeer = RakNet::RakPeerInterface::GetInstance();

	if (mPeer->Startup(12, &mSocketDescriptor, 1) != RakNet::RAKNET_STARTED)
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
			case MessageIDs::ID_CONNECTION_INIT_LHMP:
			{
				ServerConnectionHandler Handler(&mClients);
				Handler.ProcessMessage(this, packet);
			}
			break;
			case MessageIDs::ID_SYNC_LHMP:
			{
				ServerSyncHandler Handler(&mClients);
				Handler.ProcessMessage(this, packet);
			}
			break;
		}
	}
}